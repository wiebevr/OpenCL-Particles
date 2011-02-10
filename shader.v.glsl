uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
attribute vec4 vertexPosition;

void main(void)
{
    gl_FrontColor = gl_Color;

    gl_Position = projectionMatrix * modelViewMatrix *
        vec4(vertexPosition.xyz, 1.0);
}
