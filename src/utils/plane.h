#pragma once

#include <glm/glm.hpp>

class Plane
{
public:
    glm::vec4 equation;
    glm::vec3 origin;
    glm::vec3 normal;

    Plane(const glm::vec3 origin, const glm::vec3 normal);
    Plane(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3);

    bool isFrontFacingTo(const glm::vec3 direction) const { return (glm::dot(normal, direction) <= 0); }
    double signedDistanceTo(const glm::vec3 point) const { return (glm::dot(point, normal) + equation[3]); }
};

