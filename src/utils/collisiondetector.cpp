#include "collisiondetector.h"
#include "utils/plane.h"
#include <iostream>

// bool CollisionDetector::isInTriangle(const glm::vec3 point, const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3) {
//     glm::vec3 p1 = v1 - point;
//     glm::vec3 p2 = v2 - point;
//     glm::vec3 p3 = v3 - point;

//     glm::vec3 n1 = glm::cross(p1, p2);
//     glm::vec3 n2 = glm::cross(p2, p3);
//     glm::vec3 n3 = glm::cross(p3, p1);

//     if (glm::dot(n1, n2) >= 0.f && glm::dot(n1, n3) >= 0.f) {
//         return true;
//     }

//     return false;
// }

void CollisionDetector::checkCollision(CollisionPacket& collisionPacket, std::vector<bool>& worldData) {
    glm::vec3 pos = collisionPacket.pos;
    glm::vec3 dv = collisionPacket.velocity;

    // worldData indexed by x, then y, then z



    // calculate t (assume ray is pos + t * dv)



    // pos.x + t * dv.x = index - 0.5


}

// void CollisionDetector::checkCubeCollision(CollisionPacket& collisionPacket, std::vector<bool>& worldData, glm::vec3 cube) {

//     glm::vec3 pos = collisionPacket.pos;
//     glm::vec3 dv = collisionPacket.velocity;

//     float t = 1.f;

//     float negativeXTValue = ((cube.x - 0.5f) - pos.x) / dv.x;
//     glm::vec3 negativeXIntersection = pos + negativeXTValue * dv;

//     if ((negativeXTValue < t) && (negativeXTValue > 0) &&
//         (negativeXIntersection.y > (cube.y - 0.5)) &&
//         (negativeXIntersection.y < (cube.y + 0.5)) &&
//         (negativeXIntersection.z > (cube.z - 0.5)) &&
//         (negativeXIntersection.z < (cube.z + 0.5))) {

//         t = ((static_cast<float>(cube.x) - 0.5f) - pos.x) / dv.x;
//         collisionPacket.collisionFace = glm::vec3(-1.f, 0.f, 0.f);

//     }

//     float positiveXTValue = ((cube.x + 0.5f) - pos.x) / dv.x;
//     glm::vec3 positiveXIntersection = pos + positiveXTValue * dv;

//     if ((positiveXTValue < t) && (positiveXTValue > 0) &&
//         (positiveXIntersection.y > (cube.y - 0.5)) &&
//         (positiveXIntersection.y < (cube.y + 0.5)) &&
//         (positiveXIntersection.z > (cube.z - 0.5)) &&
//         (positiveXIntersection.z < (cube.z + 0.5))) {

//         t = ((static_cast<float>(cube.x) + 0.5f) - pos.x) / dv.x;
//         collisionPacket.collisionFace = glm::vec3(-1.f, 0.f, 0.f);

//     }

//     if (((static_cast<float>(y) - 0.5f) - pos.y) / dv.y < t) {
//         t = ((static_cast<float>(y) - 0.5f) - pos.y) / dv.y;
//         collisionPacket.collisionFace = glm::vec3(0.f, -1.f, 0.f);
//     }

//     if (((static_cast<float>(y) + 0.5f) - pos.y) / dv.y < t) {
//         t = ((static_cast<float>(y) + 0.5f) - pos.y) / dv.y;
//         collisionPacket.collisionFace = glm::vec3(0.f, 1.f, 0.f);
//     }

//     if (((static_cast<float>(z) - 0.5f) - pos.z) / dv.z < t) {
//         t = ((static_cast<float>(z) - 0.5f) - pos.z) / dv.z;
//         collisionPacket.collisionFace = glm::vec3(0.f, 0.f, -1.f);
//     }

//     if (((static_cast<float>(z) + 0.5f) - pos.z) / dv.z < t) {
//         t = ((static_cast<float>(z) + 0.5f) - pos.z) / dv.z;
//         collisionPacket.collisionFace = glm::vec3(0.f, 0.f, 1.f);
//     }

// }

// bool CollisionDetector::isInCube(glm::vec3 pos, glm::vec3 cubePos) {
//     return (round(pos.x) == cubePos.x) && (round(pos.y) == cubePos.y) && (round(pos.z) == cubePos.z);
// }

// void CollisionDetector::checkTriangleCollision(CollisionPacket& collisionPacket, const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3) {

//     Plane trianglePlane(v1, v2, v3);

//     if (trianglePlane.isFrontFacingTo(collisionPacket.normalizedVelocity)) {
//         return;
//     }

//     // time interval of plane intersection

//     float t0;
//     float t1;
//     bool embeddedInPlane = false;

//     float distToPlane = trianglePlane.signedDistanceTo(collisionPacket.basePoint);
//     float normalDotVelocity = glm::dot(trianglePlane.normal, collisionPacket.velocity);

//     // if velocity is parallel to plane

//     if (normalDotVelocity == 0.0f) {
//         if (abs(distToPlane) > 1.0f) {
//             return;
//         } else {
//             embeddedInPlane = true;
//             t0 = 0.f;
//             t1 = 1.f;
//         }
//     } else {
//         // Calculate t0 and t1

//         t0 = (-1.f - distToPlane) / normalDotVelocity;
//         t1 = (1.f - distToPlane) / normalDotVelocity;

//         if (t0 > t1) {
//             float tmp = t1;
//             t1 = t0;
//             t0 = tmp;
//         }

//         if (t0 > 1.f || t1 < 0.f) { return; }

//         t0 = std::fmax(0.f, std::fmin(1.f, t0));
//         t1 = std::fmax(0.f, std::fmin(1.f, t1));
//     }

//     glm::vec3 collisionPoint;
//     bool foundCollision = false;
//     float t = 1.f;

//     // collision in triangle

//     if (!embeddedInPlane) {
//         glm::vec3 planeIntersectionPoint = (collisionPacket.basePoint - trianglePlane.normal) + t0 * collisionPacket.velocity;

//         if (isInTriangle(planeIntersectionPoint, v1, v2, v3)) {
//             foundCollision = true;
//             t = t0;
//             collisionPoint = planeIntersectionPoint;
//         }
//     }

//     // if collision, check if collision is closest

//     if (foundCollision) {
//         float distToCollision = t * collisionPacket.velocity.length();

//         if ((!collisionPacket.foundCollision) || distToCollision < collisionPacket.nearestDistance) {
//             collisionPacket.nearestDistance = distToCollision;
//             collisionPacket.intersectionPoint = collisionPoint;
//             collisionPacket.foundCollision = true;
//         }
//     }

// }

// void CollisionDetector::checkWorldCollision(std::vector<float>& vertexData, CollisionPacket& collisionPacket) {
//     int i = 0;
//     while (i < vertexData.size()) {
//         glm::vec3 v1(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
//         glm::vec3 v2(vertexData[i + 6], vertexData[i + 7], vertexData[i + 8]);
//         glm::vec3 v3(vertexData[i + 12], vertexData[i + 13], vertexData[i + 14]);
//         checkTriangleCollision(collisionPacket, v1, v2, v3);
//         i += 18;
//     }
// }
