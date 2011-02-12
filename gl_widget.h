#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include <QKeyEvent>

#include "camera.h"
#include "simulator.h"

class GLWidget
    : public QGLWidget
{
    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);
        virtual ~GLWidget();

    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);

        void makeGeometry();
        void makeShaders();

        void mouseMoveEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
    protected slots:
        void step();

    private:
        Camera *_camera;

        GLuint _vertexBuffer;
        GLuint _elementBuffer;
        QGLShaderProgram *_shaderProgram;

        QTimer *_timer;

        Simulator *_simulator;

        QPoint _lastPosition;


};
#endif // GL_WIDGET_H
