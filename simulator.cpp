#include "simulator.h"

#include <QtOpenGL>
#include <CL/cl_gl_ext.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>
#define GL_SHARING_EXTENSION "cl_khr_gl_sharing"

#include <QtGlobal>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <QGLContext>
#include <cassert>
#include <cstdio>
#include <cstdlib>

const float Simulator::SIZE_OF_UNIVERSE = 10.0f;

    Simulator::Simulator(GLuint starVBO, QObject *parent)
    : QObject(parent)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(simulationStep()));


    _starVBO = starVBO;

    createInput(NUMBER_OF_STARS, NUMBER_OF_GALACTIC_CENTERS);
    initCL();
    _timer->start(20);
}

Simulator::~Simulator()
{
    clReleaseKernel(_kernels[0]);
    clReleaseProgram(_program);
    clReleaseCommandQueue(_commandQueue);
    clReleaseContext(_context);
    clReleaseMemObject(_starBuffer);
    clReleaseMemObject(_starSpeedBuffer);
    clReleaseMemObject(_galacticCenterBuffer);


    delete[] _stars;
    delete[] _galacticCenters;
    delete[] _starSpeed;

}

void Simulator::initCL()
{
    cl_int error;

    clGetPlatformIDs(1, &_platform, 0);

    error = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_GPU, 1, &_device, NULL);
    assert(error == CL_SUCCESS);

    cl_context_properties props[] =
    {
        CL_GL_CONTEXT_KHR,  (cl_context_properties)glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties)_platform,
        0
    };
    _context = clCreateContext(props, 1, &_device, NULL, NULL, &error);
    qDebug("Error code: %d\n", error);
    assert(error == CL_SUCCESS);

    _commandQueue = clCreateCommandQueue(_context, _device, 0, &error);
    assert(error == CL_SUCCESS);

    QByteArray sourceCode;
    int fileSize = readFile("kernel.cl", sourceCode);
    const char *sourceList[1];
    sourceList[0] = sourceCode.constData();
    _program = clCreateProgramWithSource(_context, 1,
            (const char **)sourceList, (size_t *)&fileSize, &error);
    assert(error == CL_SUCCESS);

    error = clBuildProgram(_program, 0, NULL, NULL, NULL, NULL);
    printBuildLog();
    assert(error == CL_SUCCESS);

    _kernels[0] = clCreateKernel(_program, "starSimulation", &error);
    assert(error == CL_SUCCESS);

    size_t starBufferSize = sizeof(cl_float4) * _numberOfStars;
    size_t galacticCenterBufferSize = sizeof(cl_float4) *
        _numberOfGalacticCenters;

    // VBO Buffer (sterren)
    _starBuffer = clCreateFromGLBuffer(_context, CL_MEM_READ_WRITE,
            _starVBO, &error);
    assert(error == CL_SUCCESS);
    clFinish(_commandQueue);
    error = clEnqueueAcquireGLObjects(_commandQueue, 1, &_starBuffer, 0,
            NULL, NULL);
    error = clEnqueueWriteBuffer(_commandQueue, _starBuffer, CL_TRUE, 0,
            starBufferSize, (void *)_stars, 0, NULL, NULL);
    assert(error == CL_SUCCESS);

    // Galactic Center Buffer
    _galacticCenterBuffer = clCreateBuffer(_context, CL_MEM_READ_WRITE,
            galacticCenterBufferSize, NULL, &error);
    error |= clEnqueueWriteBuffer(_commandQueue, _galacticCenterBuffer, CL_TRUE, 0,
            galacticCenterBufferSize, (void *)_galacticCenters, 0, NULL, NULL);
    assert(error == CL_SUCCESS);

    // Star speed buffer
    _starSpeedBuffer = clCreateBuffer(_context, CL_MEM_READ_WRITE,
            starBufferSize, NULL, &error);
    error |= clEnqueueWriteBuffer(_commandQueue, _starSpeedBuffer, CL_TRUE, 0,
            starBufferSize, (void *)_starSpeed, 0, NULL, NULL);
    assert(error == CL_SUCCESS);

    error  = clSetKernelArg(_kernels[0], 0, sizeof(int),
            &_numberOfGalacticCenters);
    error |= clSetKernelArg(_kernels[0], 1, sizeof(cl_mem),
            &_starBuffer);
    error |= clSetKernelArg(_kernels[0], 2, sizeof(cl_mem),
            &_galacticCenterBuffer);
    error |= clSetKernelArg(_kernels[0], 3, sizeof(cl_mem),
            &_starSpeedBuffer);
    assert(error == CL_SUCCESS);

    error = clEnqueueReleaseGLObjects(_commandQueue, 1, &_starBuffer, 0,
            NULL, NULL);
    assert(error == CL_SUCCESS);

    clFinish(_commandQueue);
}

void Simulator::simulationStep()
{
    cl_int error;

    const size_t workItemsPerGroup = 64;
    const size_t totalNumberOfWorkItems = (NUMBER_OF_STARS / 64 + 1) * 64;

    error = clEnqueueAcquireGLObjects(_commandQueue, 1, &_starBuffer, 0,
            NULL, NULL);
    assert(error == CL_SUCCESS);

    error = clEnqueueNDRangeKernel(_commandQueue, _kernels[0], 1, NULL,
            &totalNumberOfWorkItems, &workItemsPerGroup, 0, NULL, NULL);
    assert(error == CL_SUCCESS);

    error = clEnqueueReleaseGLObjects(_commandQueue, 1, &_starBuffer, 0,
            NULL, NULL);
    assert(error == CL_SUCCESS);

    clFinish(_commandQueue);

}

void Simulator::createInput(int numberOfStars, int numberOfGalacticCenters)
{
    _numberOfStars = numberOfStars;
    _numberOfGalacticCenters = numberOfGalacticCenters;
    _stars = new cl_float3[_numberOfStars];
    _galacticCenters = new cl_float3[_numberOfGalacticCenters];
    generateRandom(_stars, _numberOfStars);
    generateRandom(_galacticCenters, _numberOfGalacticCenters);
    _starSpeed = new cl_float4[_numberOfStars];
    memset(_starSpeed, 0, sizeof(cl_float) * _numberOfStars);
}

void Simulator::printBuildLog()
{
    size_t logSize;
    clGetProgramBuildInfo(_program, _device, CL_PROGRAM_BUILD_LOG, 0, NULL,
            &logSize);
    QByteArray log((int)logSize + 1, '\0');
    clGetProgramBuildInfo(_program, _device, CL_PROGRAM_BUILD_LOG,
            logSize, log.data(), NULL);
    qDebug() << log;


}

void Simulator::generateRandom(cl_float3 *objects, int number)
{
    qsrand(QTime::currentTime().msec());

    for (int i = 0; i < number; ++i)
    {
        objects[i].x = ((float)qrand() / (float)RAND_MAX) * SIZE_OF_UNIVERSE -
            SIZE_OF_UNIVERSE / 2;
        objects[i].y = ((float)qrand() / (float)RAND_MAX) * SIZE_OF_UNIVERSE -
            SIZE_OF_UNIVERSE / 2;
        objects[i].z = ((float)qrand() / (float)RAND_MAX) * SIZE_OF_UNIVERSE -
            SIZE_OF_UNIVERSE / 2;
    }
}

int Simulator::readFile(QString filename, QByteArray &sourceCode)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    sourceCode = file.readAll();

    return file.size();
}
