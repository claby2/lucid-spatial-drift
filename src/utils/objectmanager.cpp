#include "objectmanager.h"

void VaoManager::create() { glGenVertexArrays(1, &m_vao); }
void VaoManager::bind() const { glBindVertexArray(m_vao); }
void VaoManager::enable() const {
  // VAO for the vertex data
  // (x, y, z) position
  // (r, g, b) color
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        reinterpret_cast<void *>(0));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        reinterpret_cast<void *>(3 * sizeof(GLfloat)));
}
void VaoManager::unbind() const { glBindVertexArray(0); }
void VaoManager::free() { glDeleteVertexArrays(1, &m_vao); }

void VboManager::create() { glGenBuffers(1, &m_vbo); }
void VboManager::allocate(const std::vector<float> &vertexData) const {
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(),
               vertexData.data(), GL_STATIC_DRAW);
}
void VboManager::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_vbo); }
void VboManager::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void VboManager::free() { glDeleteBuffers(1, &m_vbo); }
