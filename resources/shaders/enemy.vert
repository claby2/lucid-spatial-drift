#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 uv_in;

out vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
    uv = uv_in;
    vec4 worldSpacePosition = (modelMatrix * vec4(vertexPos, 1.0));

    gl_Position = projectionMatrix * viewMatrix * worldSpacePosition;
}
