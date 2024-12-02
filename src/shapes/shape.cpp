#include "shape.h"
#include <iostream>

void Shape::initialize() {
  m_vao.create();
  m_vbo.create();
}

void insertVec3(std::vector<float> &data, glm::vec3 v) {
  data.push_back(v.x);
  data.push_back(v.y);
  data.push_back(v.z);
}

void Shape::updateParams(int param1, int param2) {
  if (param1 == m_param1 && param2 == m_param2)
    return;
  m_param1 = param1;
  m_param2 = param2;
  m_created = false;
}

void Shape::installVbo() {
  m_vao.bind();
  m_vbo.bind();
  m_vbo.allocate(generate());
  m_vao.enable();
  m_vbo.unbind();
  m_vao.unbind();
}

void Shape::bindVao() { m_vao.bind(); }

void Shape::unbindVao() { m_vao.unbind(); }

std::vector<float> Shape::generate() {
  if (!m_created) {
    m_vertexData.clear();
    makeShape();
    m_created = true;
  }
  return m_vertexData;
}

Shape::~Shape() {
  m_vbo.free();
  m_vao.free();
}

int Shape::getVertexCount() { return m_vertexData.size() / 6; }
