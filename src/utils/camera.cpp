#include "camera.h"

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

void Camera::collideAndSlide(std::vector<float>& vertexData, const glm::vec3 v, const glm::vec3 g) {

    // movement

    CollisionDetector::CollisionPacket collisionPackage;

    collisionPackage.R3Position = m_pos;
    collisionPackage.R3Velocity = v;
    collisionPackage.ellipsoidSize = playerSize;

    glm::vec3 ellipsoidSpacePosition = collisionPackage.R3Position / collisionPackage.ellipsoidSize;
    glm::vec3 ellipsoidSpaceVelocity = collisionPackage.R3Velocity / collisionPackage.ellipsoidSize;

    collisionPackage.basePoint = ellipsoidSpacePosition;
    collisionPackage.velocity = ellipsoidSpaceVelocity;
    collisionPackage.normalizedVelocity = glm::normalize(ellipsoidSpaceVelocity);

    glm::vec3 finalPosition = collideWithWorld(0, vertexData, collisionPackage);

    // gravity

    CollisionDetector::CollisionPacket gravityCollisionPackage;

    gravityCollisionPackage.R3Position = finalPosition * collisionPackage.ellipsoidSize;
    gravityCollisionPackage.R3Velocity = g;
    gravityCollisionPackage.ellipsoidSize = playerSize;

    ellipsoidSpacePosition = gravityCollisionPackage.R3Position / collisionPackage.ellipsoidSize;
    ellipsoidSpaceVelocity = g / gravityCollisionPackage.ellipsoidSize;

    gravityCollisionPackage.basePoint = ellipsoidSpacePosition;
    gravityCollisionPackage.velocity = ellipsoidSpaceVelocity;
    gravityCollisionPackage.normalizedVelocity = glm::normalize(ellipsoidSpaceVelocity);

    finalPosition = collideWithWorld(0, vertexData, gravityCollisionPackage);

    finalPosition *= playerSize;

    m_pos = finalPosition;
}

glm::vec3 Camera::collideWithWorld(int depth, std::vector<float>& vertexData, CollisionDetector::CollisionPacket collisionPackage) {
    float unitScale = 1.f;
    float epsilon = 0.005f * unitScale;

    glm::vec3 pos = collisionPackage.basePoint;
    glm::vec3 v = collisionPackage.velocity;

    if (depth > 5) { return pos; }

    m_collisionDetector.checkWorldCollision(vertexData, collisionPackage);

    if (!(collisionPackage.foundCollision)) { return pos + v; }

    // if collision (TODO: implement)

    glm::vec3 destinationPoint = pos + v;

    return collisionPackage.basePoint;
}
