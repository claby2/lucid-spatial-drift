#pragma once

#include <GL/glew.h>

class PostProcessor {
public:
  PostProcessor() = default;

  void initialize(int screen_width, int screen_height);
  void resize(int screen_width, int screen_height);
  void bindFramebuffer();
  void unbindFramebuffer();
  void render(bool perPixelFilter, bool kernelBasedFilter);

  ~PostProcessor();

private:
  void makeFBO();

  GLuint m_shader;
  GLuint m_defaultFBO = 2;
  GLuint m_fbo;
  GLuint m_fbo_texture;
  GLuint m_fbo_renderbuffer;
  GLuint m_vbo;
  GLuint m_vao;

  int m_screen_width;
  int m_screen_height;
};
