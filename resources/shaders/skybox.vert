#version 330 core

layout (location = 0) in vec3 position;

out vec3 textureCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main(void)
{
    textureCoordinates = position;
    gl_Position = projection * view * vec4(position, 1);
}
