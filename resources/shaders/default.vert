#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;

uniform float time;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 worldSpaceNormal;
out vec3 worldSpacePosition;

void main() {
    worldSpacePosition = (modelMatrix * vec4(vertexPos, 1.0)).xyz;
    worldSpacePosition.y += sin(worldSpacePosition.x * 0.5 + time) * 0.2;
    worldSpacePosition.x += cos(worldSpacePosition.y * 0.5 + time) * 0.2;
    worldSpacePosition.z += sin(worldSpacePosition.y * worldSpacePosition.x * 0.5 + time * 0.2) * 0.05;

    worldSpaceNormal = normalMatrix * normalize(vertexNormal);

    gl_Position = projectionMatrix * viewMatrix * vec4(worldSpacePosition, 1.0);
}
