#include "plane.h"

Plane::Plane(const glm::vec3 origin, const glm::vec3 normal) {
    this->normal = normal;
    this->origin = origin;
    equation[0] = normal.x;
    equation[1] = normal.y;
    equation[2] = normal.z;
    equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}

Plane::Plane(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3) {
    normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
    origin = p1;
    equation[0] = normal.x;
    equation[1] = normal.y;
    equation[2] = normal.z;
    equation[3] = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
}
