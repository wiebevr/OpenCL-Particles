#include "camera.h"

Camera::Camera(double angle, double aspect, double nearPlane, 
        double farPlane, QObject *parent)
    : QObject(parent)
{
    _cameraAngle = angle;
    _aspectRatio = aspect;
    _nearPlane = nearPlane;
    _farPlane = farPlane;
    updateProjectionMatrix();
}

Camera::~Camera()
{
}

const QMatrix4x4 &Camera::getModelViewMatrix() const
{
    return _modelViewMatrix;
}

const QMatrix4x4 &Camera::getProjectionMatrix() const
{
    return _projectionMatrix;
}

const QMatrix4x4 &Camera::getRotationMatrix() const
{
    return _rotationMatrix;
}

void Camera::resize(int width, int height)
{
    _aspectRatio = (double)width / (double)height;
    updateProjectionMatrix();
}

void Camera::rotate(double angle, const QVector3D &axis)
{
    QMatrix4x4 delta;
    delta.rotate(angle, axis);
    _rotationMatrix = delta * _rotationMatrix;
    updateModelViewMatrix();
}

void Camera::setRotation(double angle, const QVector3D &axis)
{
    _rotationMatrix.setToIdentity();
    _rotationMatrix.rotate(angle, axis);
    updateModelViewMatrix();
}

void Camera::translate(const QVector3D &direction)
{
    _position += direction;
    updateModelViewMatrix();
}

void Camera::setPosition(const QVector3D &position)
{
    _position = position;
    updateModelViewMatrix();
}

void Camera::translateRotationPoint(const QVector3D &direction)
{
    _rotationPoint += direction;
    updateModelViewMatrix();
}

void Camera::setRotationPoint(const QVector3D &position)
{
    _rotationPoint = position;
    updateModelViewMatrix();
}

void Camera::reset()
{
    updateProjectionMatrix();

    _modelViewMatrix.setToIdentity();
    _rotationMatrix.setToIdentity();
    
    _rotationPoint = QVector3D(0.0f, 0.0f, 0.0f);
    _position = QVector3D(0.0f, 0.0f, 0.0f);
}

void Camera::updateModelViewMatrix()
{
    _modelViewMatrix.setToIdentity();
    _modelViewMatrix.translate(-_rotationPoint);
    _modelViewMatrix = _modelViewMatrix * _rotationMatrix;
    _modelViewMatrix.translate(_position);
}

void Camera::updateProjectionMatrix()
{
    _projectionMatrix.setToIdentity();
    _projectionMatrix.perspective(_cameraAngle, _aspectRatio, _nearPlane, 
            _farPlane);
}
