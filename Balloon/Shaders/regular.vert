#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

layout(std140) uniform GlobalMatrices
{
    mat4 cameraToClipMatrix;
    mat4 worldToCameraMatrix;
};

uniform mat4 localToWorldMatrix;

smooth out vec4 theColor;

void main()
{
    vec4 temp = localToWorldMatrix * position;
    temp = worldToCameraMatrix * temp;
    gl_Position = cameraToClipMatrix * temp;

	theColor = color;
}