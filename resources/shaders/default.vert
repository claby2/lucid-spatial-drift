#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 worldSpaceNormal;
out vec3 worldSpacePosition;

void main() {
    worldSpacePosition = (modelMatrix * vec4(vertexPos, 1.0)).xyz;
    worldSpaceNormal = normalMatrix * normalize(vertexNormal);

    gl_Position = projectionMatrix * viewMatrix * vec4(worldSpacePosition, 1.0);
}
