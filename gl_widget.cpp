#include "gl_widget.h"
#include <QVector3D>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    _camera = new Camera(60.0, (double)width() / (double)height(),
            1.0, 800.0, this);
    _camera->setRotationPoint(QVector3D(0.0f, 0.0f, 20.0f));

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(step()));

    //Simulator simulator(this->context());
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(2.0f);

    makeGeometry();
    makeShaders();

    _simulator = new Simulator(_vertexBuffer, this);
    _timer->start(30);
}

void GLWidget::step()
{
    _simulator->simulationStep();
    updateGL();
}

void GLWidget::paintGL()
{
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shaderProgram->bind();

    _shaderProgram->setUniformValue(
            "projectionMatrix",
            _camera->getProjectionMatrix());
    _shaderProgram->setUniformValue(
            "modelViewMatrix",
            _camera->getModelViewMatrix());

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    _shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 4);
    _shaderProgram->enableAttributeArray("vertexPosition");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);

    glDrawElements(
        GL_POINTS,
        Simulator::NUMBER_OF_STARS,
        GL_UNSIGNED_INT,
        (void *)0);

}

void GLWidget::resizeGL(int width, int height)
{
    _camera->resize(width, height);
    glViewport(0, 0, width, height);
}

void GLWidget::makeGeometry()
{
    //const GLfloat vertexData[Simulator::NUMBER_OF_STARS * 4] = {0};
    GLuint elementData[Simulator::NUMBER_OF_STARS];
    for (int i = 0; i < Simulator::NUMBER_OF_STARS; ++i)
        elementData[i] = i;

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            Simulator::NUMBER_OF_STARS * sizeof(cl_float4),
            /*(void *)vertexData*/0, GL_STATIC_DRAW);
    qDebug() << "glBufferSize =" << Simulator::NUMBER_OF_STARS * sizeof(cl_float4);

    glGenBuffers(1, &_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            Simulator::NUMBER_OF_STARS * sizeof(GLuint), 
            (void *)elementData, GL_STATIC_DRAW);
}

void GLWidget::makeShaders()
{
    _shaderProgram = new QGLShaderProgram(this);

    QGLShader *vertexShader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fragmentShader = new QGLShader(QGLShader::Fragment, this);

    vertexShader->compileSourceFile("shader.v.glsl");
    fragmentShader->compileSourceFile("shader.f.glsl");

    if (!vertexShader->isCompiled())
    {
        qDebug()
            << "Could not compile vertex shader:\n"
            << vertexShader->log();
    }

    if (!fragmentShader->isCompiled())
    {
        qDebug()
            << "Could not compile fragment shader:\n"
            << fragmentShader->log();
    }

    _shaderProgram->addShader(fragmentShader);
    _shaderProgram->addShader(vertexShader);
    _shaderProgram->link();

}
