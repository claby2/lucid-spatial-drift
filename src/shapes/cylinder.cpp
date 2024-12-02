#include "cylinder.h"

void Cylinder::makeBodyTile(glm::vec3 topLeft, glm::vec3 topRight,
                            glm::vec3 bottomLeft, glm::vec3 bottomRight) {
  auto add = [this](const glm::vec3 &v) {
    insertVec3(m_vertexData, v);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(v.x, 0.0f, v.z)));
  };
  add(topLeft);
  add(bottomLeft);
  add(topRight);

  add(topRight);
  add(bottomLeft);
  add(bottomRight);
}

void Cylinder::makeCapTile(glm::vec3 topLeft, glm::vec3 topRight,
                           glm::vec3 bottomLeft, glm::vec3 bottomRight,
                           glm::vec3 normal) {
  auto add = [this, normal](const glm::vec3 &v) {
    insertVec3(m_vertexData, v);
    insertVec3(m_vertexData, normal);
  };
  add(topLeft);
  add(bottomLeft);
  add(topRight);

  add(topRight);
  add(bottomLeft);
  add(bottomRight);
}

void Cylinder::makeCap(float y, glm::vec3 normal) {
  float size = 0.5f / m_param1;
  float step = 2.0f * M_PI / m_param2;

  for (int i = 0; i < m_param1; i++) {
    float lastRadius = i * size;
    float currentRadius = (i + 1) * size;

    for (int j = 0; j < m_param2; j++) {
      float currentTheta = j * step;
      float nextTheta = (j + 1) * step;

      glm::vec3 tl = glm::vec3(lastRadius * glm::cos(currentTheta), y,
                               lastRadius * glm::sin(currentTheta));
      glm::vec3 tr = glm::vec3(lastRadius * glm::cos(nextTheta), y,
                               lastRadius * glm::sin(nextTheta));
      glm::vec3 bl = glm::vec3(currentRadius * glm::cos(currentTheta), y,
                               currentRadius * glm::sin(currentTheta));
      glm::vec3 br = glm::vec3(currentRadius * glm::cos(nextTheta), y,
                               currentRadius * glm::sin(nextTheta));

      if (y < 0.0f) {
        makeCapTile(tl, tr, bl, br, normal);
      } else {
        makeCapTile(tr, tl, br, bl, normal);
      };
    }
  }
}

void Cylinder::makeBody() {
  float size = 1.0f / m_param1;
  float step = 2.0f * M_PI / m_param2;
  for (int i = 0; i < m_param1; i++) {
    float lastY = i * size - 0.5f;
    float nextY = (i + 1) * size - 0.5f;

    for (int j = 0; j < m_param2; j++) {
      float currentTheta = j * step;
      float nextTheta = (j + 1) * step;
      glm::vec3 tl = glm::vec3(0.5f * glm::cos(currentTheta), lastY,
                               0.5f * glm::sin(currentTheta));
      glm::vec3 tr = glm::vec3(0.5f * glm::cos(nextTheta), lastY,
                               0.5f * glm::sin(nextTheta));
      glm::vec3 bl = glm::vec3(0.5f * glm::cos(currentTheta), nextY,
                               0.5f * glm::sin(currentTheta));
      glm::vec3 br = glm::vec3(0.5f * glm::cos(nextTheta), nextY,
                               0.5f * glm::sin(nextTheta));
      makeBodyTile(tl, tr, bl, br);
    }
  }
}

void Cylinder::makeShape() {
  m_param2 = std::max(m_param2, 3);

  makeCap(-0.5f, glm::vec3(0.0f, -1.0f, 0.0f));
  makeCap(0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

  makeBody();
}
