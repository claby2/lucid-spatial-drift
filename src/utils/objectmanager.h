#pragma once

#include <GL/glew.h>
#include <vector>

class VaoManager {
public:
  VaoManager() = default;

  void create();
  void bind() const;
  void enable() const;
  void unbind() const;

  void free();

private:
  GLuint m_vao;
};

class VboManager {
public:
  VboManager() = default;

  void create();
  void bind() const;
  void allocate(const std::vector<float> &vertexData) const;
  void unbind() const;

  void free();

private:
  GLuint m_vbo;
};
