#version 330 core

out vec4 FragColor;

in vec3 textureCoordinates;

uniform samplerCube skybox;

void main(void)
{
    FragColor = texture(skybox, textureCoordinates);
}
