#pragma once

#include "utils/objectmanager.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

void insertVec3(std::vector<float> &data, glm::vec3 v);

class Shape {
public:
  Shape() = default;
  void initialize();
  void updateParams(int param1, int param2);
  void installVbo();
  void bindVao();
  void unbindVao();
  std::vector<float> generate();
  int getVertexCount();

  virtual void makeShape() = 0;

  ~Shape();

protected:
  int m_param1;
  int m_param2;
  bool m_created = false;
  std::vector<float> m_vertexData;

private:
  VaoManager m_vao;
  VboManager m_vbo;
};
