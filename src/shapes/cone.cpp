#include "cone.h"
#include <iostream>

void Cone::makeBodyTile(glm::vec3 topLeft, glm::vec3 topRight,
                        glm::vec3 bottomLeft, glm::vec3 bottomRight,
                        bool is_top) {
  auto normal = [](glm::vec3 p) {
    return glm::normalize(
        glm::vec3(2.0f * p.x, -0.25f * (2.0f * p.y - 1.0f), 2.0f * p.z));
  };

  glm::vec3 bottomLeftNormal = normal(bottomLeft);
  glm::vec3 bottomRightNormal = normal(bottomRight);
  glm::vec3 topLeftNormal = normal(topLeft);
  if (is_top) {
    // y = 0.5
    glm::vec3 xz = (normal(bottomLeft) + normal(bottomRight)) / 2.0f;
    xz.y = 0.0f;
    xz = glm::normalize(xz);
    xz.y = 0.5f;
    xz = glm::normalize(xz);
    topLeftNormal = xz;
  }
  glm::vec3 topRightNormal = normal(topRight);
  if (is_top) {
    glm::vec3 xz = (normal(bottomLeft) + normal(bottomRight)) / 2.0f;
    xz.y = 0.0f;
    xz = glm::normalize(xz);
    xz.y = 0.5f;
    xz = glm::normalize(xz);
    topRightNormal = xz;
  }

  insertVec3(m_vertexData, topLeft);
  insertVec3(m_vertexData, topLeftNormal);
  insertVec3(m_vertexData, bottomLeft);
  insertVec3(m_vertexData, bottomLeftNormal);
  insertVec3(m_vertexData, bottomRight);
  insertVec3(m_vertexData, bottomRightNormal);

  insertVec3(m_vertexData, topLeft);
  insertVec3(m_vertexData, topLeftNormal);
  insertVec3(m_vertexData, bottomRight);
  insertVec3(m_vertexData, bottomRightNormal);
  insertVec3(m_vertexData, topRight);
  insertVec3(m_vertexData, topRightNormal);
}

void Cone::makeCapTile(glm::vec3 topLeft, glm::vec3 topRight,
                       glm::vec3 bottomLeft, glm::vec3 bottomRight,
                       glm::vec3 normal) {
  auto add = [this, normal](const glm::vec3 &v) {
    insertVec3(m_vertexData, v);
    insertVec3(m_vertexData, normal);
  };

  add(topLeft);
  add(bottomLeft);
  add(bottomRight);

  add(topLeft);
  add(bottomRight);
  add(topRight);
}

void Cone::makeCap() {
  float size = 0.5f / m_param1;
  float step = 2.0f * M_PI / m_param2;

  for (int i = 0; i < m_param1; i++) {
    float lastRadius = i * size;
    float currentRadius = (i + 1) * size;

    for (int j = 0; j < m_param2; j++) {
      float currentTheta = j * step;
      float nextTheta = (j + 1) * step;

      glm::vec3 tl = glm::vec3(lastRadius * glm::cos(currentTheta), -0.5f,
                               lastRadius * glm::sin(currentTheta));
      glm::vec3 tr = glm::vec3(lastRadius * glm::cos(nextTheta), -0.5f,
                               lastRadius * glm::sin(nextTheta));
      glm::vec3 bl = glm::vec3(currentRadius * glm::cos(currentTheta), -0.5f,
                               currentRadius * glm::sin(currentTheta));
      glm::vec3 br = glm::vec3(currentRadius * glm::cos(nextTheta), -0.5f,
                               currentRadius * glm::sin(nextTheta));

      makeCapTile(tl, tr, bl, br, glm::vec3(0.0f, -1.0f, 0.0f));
    }
  }
}

void Cone::makeBody() {
  float size = 1.0f / m_param1;
  float step = 2.0f * M_PI / m_param2;
  for (int i = 0; i < m_param1; i++) {
    float lastY = i * size - 0.5f;
    float nextY = (i + 1) * size - 0.5f;
    float lastRadius = (0.5f - lastY) / 2.0f;
    float currentRadius = (0.5f - nextY) / 2.0f;

    bool is_top = i == m_param1 - 1;
    for (int j = 0; j < m_param2; j++) {
      float currentTheta = j * step;
      float nextTheta = (j + 1) * step;
      glm::vec3 tl = glm::vec3(currentRadius * glm::cos(nextTheta), nextY,
                               currentRadius * glm::sin(nextTheta));
      glm::vec3 tr = glm::vec3(currentRadius * glm::cos(currentTheta), nextY,
                               currentRadius * glm::sin(currentTheta));
      glm::vec3 bl = glm::vec3(lastRadius * glm::cos(nextTheta), lastY,
                               lastRadius * glm::sin(nextTheta));
      glm::vec3 br = glm::vec3(lastRadius * glm::cos(currentTheta), lastY,
                               lastRadius * glm::sin(currentTheta));
      makeBodyTile(tl, tr, bl, br, is_top);
    }
  }
}

void Cone::makeShape() {
  m_param2 = std::max(m_param2, 3);
  makeCap();
  makeBody();
}
