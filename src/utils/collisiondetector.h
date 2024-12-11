#pragma once
#include <glm/glm.hpp>
#include <vector>

class CollisionDetector
{
public:

    CollisionDetector() = default;

    struct CollisionPacket {
        glm::vec3 ellipsoidSize;

        // World space

        glm::vec3 R3Velocity;
        glm::vec3 R3Position;

        // Ellipsoid space

        glm::vec3 velocity;
        glm::vec3 normalizedVelocity;
        glm::vec3 basePoint;

        // Collision info

        bool foundCollision = false;
        float nearestDistance;
        glm::vec3 intersectionPoint;
    };

    // glm::mat3 ellipsoidSpaceMatrix = glm::mat3(glm::vec3(1.f / playerSize.x, 0.f, 0.f),
    //                                            glm::vec3(0.f, 1.f / playerSize.y, 0.f),
    //                                            glm::vec3(0.f, 0.f, 1.f / playerSize.z));

    // glm::vec3 worldSpaceToEllipsoidSpace(const glm::vec3 vec) { return ellipsoidSpaceMatrix * vec; }

    // glm::vec3 ellipsoidSpaceToWorldSpace(const glm::vec3 vec) { return glm::inverse(ellipsoidSpaceMatrix) * vec; }

    bool isInTriangle(const glm::vec3 point, const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3);

    void checkTriangleCollision(CollisionPacket& collisionPacket, const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3);

    void checkWorldCollision(std::vector<float>& vertexData, CollisionPacket& collisionPacket);
};
