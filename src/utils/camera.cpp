#include "camera.h"
#include <optional>

glm::mat4 perspective(float fovy, float aspect, float near, float far) {

  glm::mat4 scaleMat =
      glm::mat4(1.0f / (aspect * far * glm::tan(fovy / 2)), 0.0f, 0.0f, 0.0f, //
                0.0f, 1.0f / (far * glm::tan(fovy / 2)), 0.0f, 0.0f,          //
                0.0f, 0.0f, 1.0 / far, 0.0,                                   //
                0.0f, 0.0f, 0.0f, 1.0f                                        //
      );
  float c = -near / far;

  glm::mat4 unhingeMat = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,               //
                                   0.0f, 1.0f, 0.0f, 0.0f,               //
                                   0.0f, 0.0f, 1.0f / (1.0f + c), -1.0f, //
                                   0.0f, 0.0f, -c / (1.0f + c), 0.0f     //
  );

  // adjust for OpenGL coordinates
  glm::mat4 adjustMat = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,  //
                                  0.0f, 1.0f, 0.0f, 0.0f,  //
                                  0.0f, 0.0f, -2.0f, 0.0f, //
                                  0.0f, 0.0f, -1.0f, 1.0f  //
  );

  return adjustMat * unhingeMat * scaleMat;
}

glm::mat4 Camera::getView() const {
  glm::vec3 w = glm::normalize(-m_look);
  glm::vec3 v = glm::normalize(m_up - (glm::dot(m_up, w) * w));
  glm::vec3 u = glm::cross(v, w);

  glm::mat4 transMat = glm::mat4(1.f, 0.f, 0.f, 0.0f, //
                                 0.f, 1.f, 0.f, 0.0f, //
                                 0.f, 0.f, 1.f, 0.0f, //
                                 -m_pos.x, -m_pos.y, -m_pos.z, 1.0f);

  glm::mat4 rotMat = glm::mat4(u.x, v.x, w.x, 0.0f, //
                               u.y, v.y, w.y, 0.0f, //
                               u.z, v.z, w.z, 0.0f, //
                               0.0f, 0.0f, 0.0f, 1.0f);

  return rotMat * transMat;
}

glm::mat4 Camera::getProjection() const {
  return perspective(m_heightAngle, m_aspectRatio, m_nearPlane, m_farPlane);
}

glm::vec3 Camera::getLook() const { return m_look; }
glm::vec3 Camera::getUp() const { return m_up; }

void Camera::setData(const SceneCameraData &data) {
  m_pos = glm::vec3(data.pos);
  m_look = glm::vec3(data.look);
  m_up = glm::vec3(data.up);
  m_heightAngle = data.heightAngle;
}
void Camera::setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
void Camera::setFarPlane(float farPlane) { m_farPlane = farPlane; }
void Camera::setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
void Camera::movePosition(glm::vec3 delta) { m_pos += delta; }
void Camera::rotate(float angle, glm::vec3 axis) {
  float t = angle;
  glm::vec3 u = glm::normalize(axis);

  glm::mat4 rotMat = glm::mat4(cos(t) + u.x * u.x * (1.0f - cos(t)),
                               u.x * u.y * (1.0f - cos(t)) + u.z * sin(t),
                               u.x * u.z * (1.0f - cos(t)) - u.y * sin(t), 0.0f,
                               u.x * u.y * (1.0f - cos(t)) - u.z * sin(t),
                               cos(t) + u.y * u.y * (1 - cos(t)),
                               u.y * u.z * (1.0f - cos(t)) + u.x * sin(t), 0.0f,
                               u.x * u.z * (1.0f - cos(t)) + u.y * sin(t),
                               u.y * u.z * (1.0f - cos(t)) - u.x * sin(t),
                               cos(t) + u.z * u.z * (1.0f - cos(t)), 1.0f, 0.0f,
                               0.0f, 0.0f, 1.0f);

  m_look = glm::vec3(rotMat * glm::vec4(m_look, 0.0f));
  m_up = glm::vec3(rotMat * glm::vec4(m_up, 0.0f));
}

void Camera::move(std::vector<bool>& worldData, float dt) {
    m_vel += dt * m_accel;

    glm::vec3 dv = dt * m_vel;

    // update position, accounting for collisions

    updatePosition(worldData, dv);

    m_vel = 0.99f * m_vel;
}

bool Camera::cubeExists(std::vector<bool>& worldData, std::optional<int> idx) {
    if (idx) {
        return worldData[idx.value()];
    } else {
        return false;
    }
}

std::optional<int> Camera::index(int x, int y, int z) {

    int WORLD_DIMENSION = 72;

    return x + y * WORLD_DIMENSION + z * WORLD_DIMENSION * WORLD_DIMENSION;
}

void Camera::updatePosition(std::vector<bool>& worldData, const glm::vec3 dv) {

    // movement

    glm::vec3 pos = m_pos;
    float epsilon = 0.01f;

    // CollisionDetector::CollisionPacket collisionPackage;

    // collisionPackage.pos = m_pos;
    // collisionPackage.velocity = dv;

    // collisionPackage.ellipsoidSize = playerSize;

    // glm::vec3 ellipsoidSpacePosition = collisionPackage.R3Position / collisionPackage.ellipsoidSize;
    // glm::vec3 ellipsoidSpaceVelocity = collisionPackage.R3Velocity / collisionPackage.ellipsoidSize;

    // collisionPackage.basePoint = ellipsoidSpacePosition;
    // collisionPackage.velocity = ellipsoidSpaceVelocity;
    // collisionPackage.normalizedVelocity = glm::normalize(ellipsoidSpaceVelocity);

    // m_collisionDetector.checkCollision(collisionPackage, worldData);

    glm::vec3 newPos = pos + dv;

    int x = round(pos.x);
    int y = round(pos.y);
    int z = round(pos.z);

    if (newPos.x > x && (cubeExists(worldData, index(x + 1, y, z)) || cubeExists(worldData, index(x + 1, y - 1, z)))) {
        newPos.x = x - epsilon;
        m_vel.x = 0.f;
    }

    if (newPos.x < x && (cubeExists(worldData, index(x - 1, y, z)) || cubeExists(worldData, index(x - 1, y - 1, z)))) {
        newPos.x = x + epsilon;
        m_vel.x = 0.f;
    }

    if (newPos.y > y && cubeExists(worldData, index(x, y + 1, z))) {
        newPos.y = y - epsilon;
        m_vel.y = 0.f;
    }

    if (newPos.y < y && cubeExists(worldData, index(x, y - 2, z))) {
        newPos.y = y + epsilon;
        m_vel.y = 0.f;
    }

    if (newPos.z > z && (cubeExists(worldData, index(x, y, z + 1)) || cubeExists(worldData, index(x, y - 1, z + 1)))) {
        newPos.z = z - epsilon;
        m_vel.z = 0.f;
    }

    if (newPos.z < z && (cubeExists(worldData, index(x, y, z - 1)) || cubeExists(worldData, index(x, y - 1, z - 1)))) {
        newPos.z = z + epsilon;
        m_vel.z = 0.f;
    }

    m_pos = newPos;

    // glm::vec3 finalPosition = collideWithWorld(0, vertexData, collisionPackage);

    // gravity

    // CollisionDetector::CollisionPacket gravityCollisionPackage;

    // gravityCollisionPackage.R3Position = finalPosition * collisionPackage.ellipsoidSize;
    // gravityCollisionPackage.R3Velocity = dg;
    // gravityCollisionPackage.ellipsoidSize = playerSize;

    // ellipsoidSpacePosition = gravityCollisionPackage.R3Position / collisionPackage.ellipsoidSize;
    // ellipsoidSpaceVelocity = dg / gravityCollisionPackage.ellipsoidSize;

    // gravityCollisionPackage.basePoint = ellipsoidSpacePosition;
    // gravityCollisionPackage.velocity = ellipsoidSpaceVelocity;
    // gravityCollisionPackage.normalizedVelocity = glm::normalize(ellipsoidSpaceVelocity);

    // finalPosition = collideWithWorld(0, vertexData, gravityCollisionPackage);

    // finalPosition *= playerSize;

    // m_pos = finalPosition;
}

// glm::vec3 Camera::collideWithWorld(int depth, std::vector<float>& vertexData, CollisionDetector::CollisionPacket collisionPackage) {
//     float unitScale = 1.f;
//     float epsilon = 0.005f * unitScale;

//     glm::vec3 pos = collisionPackage.basePoint;
//     glm::vec3 v = collisionPackage.velocity;

//     if (depth > 5) { return pos; } // unused for now

//     m_collisionDetector.checkWorldCollision(vertexData, collisionPackage);

//     if (!(collisionPackage.foundCollision)) { return pos + v; }
//     else {
//         m_vel = glm::vec3(0.f);
//         return pos;
//     }

//     // if collision (TODO: implement sliding)

//     glm::vec3 destinationPoint = pos + v;

//     return pos;
// }

void Camera::attemptJump() {
    m_vel = glm::vec3(0.f, 14.f, 0.f);
}
