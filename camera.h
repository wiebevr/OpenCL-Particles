#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>

class Camera 
    : public QObject
{
    Q_OBJECT

    public:
        Camera(double angle, double aspect, double nearPlane, 
                double farPlane, QObject *parent = 0);
        ~Camera();

        const QMatrix4x4 &getModelViewMatrix() const;
        const QMatrix4x4 &getProjectionMatrix() const;
        const QMatrix4x4 &getRotationMatrix() const;

        void rotate(double angle, const QVector3D &axis);
        void setRotation(double angle, const QVector3D &axis);
        void translate(const QVector3D &direction);
        void setPosition(const QVector3D &position);
        void translateRotationPoint(const QVector3D &direction);
        void setRotationPoint(const QVector3D &center);
        void resize(int width, int height);
//        void lookAt(const QVector3D &eye, const QVector3D &center, 
//                const QVector3D &up);
        void reset();

    protected:
        void updateModelViewMatrix();
        void updateProjectionMatrix();

    private:
        QMatrix4x4 _modelViewMatrix;
        QMatrix4x4 _projectionMatrix;
        QMatrix4x4 _rotationMatrix;

        QVector3D _rotationPoint;
        QVector3D _position;

        double _cameraAngle;
        double _aspectRatio;
        double _nearPlane;
        double _farPlane;

};
#endif // CAMERA_H
