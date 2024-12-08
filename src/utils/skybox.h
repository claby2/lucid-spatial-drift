#pragma once

#include "camera.h"
#include <GL/glew.h>
#include <QImage>

class Skybox {
public:
  Skybox() = default;

  void initialize();

  void render(const Camera &camera);

  ~Skybox();

private:
  GLuint m_shader;
  GLuint m_texture;
  GLuint m_vao;
  GLuint m_vbo;

  QImage m_image1;
  QImage m_image2;
  QImage m_image3;
  QImage m_image4;
  QImage m_image5;
  QImage m_image6;
};
