#ifndef SIMULATOR_H
#define SIMULATOR_H

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QGLContext>

class Simulator
    : public QObject
{
    Q_OBJECT

    public:
        Simulator(GLuint startVBO, QObject *parent = 0);
        virtual ~Simulator();

    public:
        static const float SIZE_OF_UNIVERSE;
        enum {NUMBER_OF_STARS = 500000, NUMBER_OF_GALACTIC_CENTERS = 10};

    public slots:
        void simulationStep();

    protected:
        void initCL();
        int readFile(QString filename, QByteArray &sourceCode);
        void printBuildLog();

        void generateRandom(cl_float3 *objects, int number, int boundary);
        void createInput(int numberOfStars, int numberOfGalacticCenters);

    private:
        cl_platform_id _platform;
        cl_context _context;
        cl_command_queue _commandQueue;
        cl_device_id _device;
        cl_program _program;
        cl_kernel _kernels[2];

        cl_mem _starBuffer;
        cl_mem _starSpeedBuffer;
        cl_mem _galacticCenterBuffer;

        cl_float4 *_stars;
        cl_float4 *_galacticCenters;
        cl_float4 *_starSpeed;
        int _numberOfStars;
        int _numberOfGalacticCenters;
        float _tijd;

        QTimer *_timer;

        GLuint _starVBO;

};
#endif // SIMULATOR_H
