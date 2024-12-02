#include "sphere.h"

const float RADIUS = 0.5f;

glm::vec3 sphereCoordinate(float r, float theta, float phi) {
  return glm::vec3(r * glm::sin(phi) * glm::cos(theta), r * glm::cos(phi),
                   r * glm::sin(phi) * glm::sin(theta));
}

void Sphere::makeTile(glm::vec3 topLeft, glm::vec3 topRight,
                      glm::vec3 bottomLeft, glm::vec3 bottomRight) {
  auto add = [this](const glm::vec3 &v) {
    insertVec3(m_vertexData, v);
    insertVec3(m_vertexData, glm::normalize(v));
  };

  add(topLeft);
  add(bottomLeft);
  add(bottomRight);

  add(topLeft);
  add(bottomRight);
  add(topRight);
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
  float step = M_PI / m_param1;

  for (int i = 0; i < m_param1; i++) {
    float phi = i * step;
    float nextPhi = (i + 1) * step;
    glm::vec3 tl = sphereCoordinate(RADIUS, currentTheta, nextPhi);
    glm::vec3 tr = sphereCoordinate(RADIUS, nextTheta, nextPhi);
    glm::vec3 bl = sphereCoordinate(RADIUS, currentTheta, phi);
    glm::vec3 br = sphereCoordinate(RADIUS, nextTheta, phi);
    makeTile(tl, tr, bl, br);
  }
}

void Sphere::makeShape() {
  m_param1 = std::max(m_param1, 2);
  m_param2 = std::max(m_param2, 3);

  float step = 2.0f * M_PI / m_param2;

  for (int i = 0; i < m_param2; i++) {
    float currentTheta = i * step;
    float nextTheta = (i + 1) * step;
    makeWedge(currentTheta, nextTheta);
  }
}
