#include "cube.h"

void Cube::makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
  insertVec3(m_vertexData, topLeft);
  insertVec3(m_vertexData, glm::normalize(glm::cross(bottomLeft - topLeft,
                                                     bottomRight - topLeft)));
  insertVec3(m_vertexData, bottomLeft);
  insertVec3(m_vertexData, glm::normalize(glm::cross(bottomRight - bottomLeft,
                                                     topLeft - bottomLeft)));
  insertVec3(m_vertexData, bottomRight);
  insertVec3(m_vertexData,
             glm::normalize(
                 glm::cross(topLeft - bottomRight, bottomLeft - bottomRight)));

  insertVec3(m_vertexData, topLeft);
  insertVec3(m_vertexData, glm::normalize(glm::cross(bottomRight - topLeft,
                                                     topRight - topLeft)));
  insertVec3(m_vertexData, bottomRight);
  insertVec3(m_vertexData, glm::normalize(glm::cross(topRight - bottomRight,
                                                     topLeft - bottomRight)));
  insertVec3(m_vertexData, topRight);
  insertVec3(m_vertexData, glm::normalize(glm::cross(topLeft - topRight,
                                                     bottomRight - topRight)));
}

void Cube::makeFace(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
  float size = 1.0f / m_param1;

  glm::vec3 colDelta = (topRight - topLeft) * size;
  glm::vec3 rowDelta = (bottomLeft - topLeft) * size;

  for (int col = 0; col < m_param1; col++) {
    for (int row = 0; row < m_param1; row++) {
      glm::vec3 tl = topLeft + (float)col * colDelta + (float)row * rowDelta;
      glm::vec3 tr = tl + colDelta;
      glm::vec3 bl = tl + rowDelta;
      glm::vec3 br = bl + colDelta;

      makeTile(tl, tr, bl, br);
    }
  }
}

void Cube::makeShape() {
  makeFace(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
           glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f));

  makeFace(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
           glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f));

  makeFace(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
           glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f));

  makeFace(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
           glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f));

  makeFace(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
           glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f));

  makeFace(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
           glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f));
}
