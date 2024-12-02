#include "postprocessor.h"
#include "shaderloader.h"
#include "utils/debug.h"

#include <GL/glew.h>

void PostProcessor::initialize(int screen_width, int screen_height) {
  m_screen_width = screen_width;
  m_screen_height = screen_height;

  glActiveTexture(GL_TEXTURE0);
  m_shader =
      ShaderLoader::createShaderProgram(":/resources/shaders/postprocess.vert",
                                        ":/resources/shaders/postprocess.frag");
  glUseProgram(m_shader);
  GLint samplerLoc = glGetUniformLocation(m_shader, "sampler");
  glUniform1i(samplerLoc, 0);
  glUseProgram(0);

  std::vector<GLfloat> fullscreen_quad_data = {
      -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, //
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //
      1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, //
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f};

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size() * sizeof(GLfloat),
               fullscreen_quad_data.data(), GL_STATIC_DRAW);
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        reinterpret_cast<void *>(3 * sizeof(GLfloat)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  makeFBO();
}

void PostProcessor::resize(int screen_width, int screen_height) {
  if (screen_width == m_screen_width && screen_height == m_screen_height) {
    return;
  }

  glDeleteTextures(1, &m_fbo_texture);
  glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
  glDeleteFramebuffers(1, &m_fbo);

  makeFBO();
}

void PostProcessor::bindFramebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glViewport(0, 0, m_screen_width, m_screen_height);
}

void PostProcessor::unbindFramebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void PostProcessor::render(bool perPixelFilter, bool kernelBasedFilter) {
  glUseProgram(m_shader);

  GLint perPixelFilterLoc = glGetUniformLocation(m_shader, "perPixelFilter");
  glUniform1i(perPixelFilterLoc, perPixelFilter);

  GLint kernelBasedFilterLoc =
      glGetUniformLocation(m_shader, "kernelBasedFilter");
  glUniform1i(kernelBasedFilterLoc, kernelBasedFilter);

  GLint screenWidthloc = glGetUniformLocation(m_shader, "screenWidth");
  glUniform1i(screenWidthloc, m_screen_width);

  GLint screenHeightloc = glGetUniformLocation(m_shader, "screenHeight");
  glUniform1i(screenHeightloc, m_screen_height);

  glBindVertexArray(m_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void PostProcessor::makeFBO() {
  glGenTextures(1, &m_fbo_texture);
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screen_width, m_screen_height, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &m_fbo_renderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screen_width,
                        m_screen_height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_fbo_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, m_fbo_renderbuffer);

  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

PostProcessor::~PostProcessor() {
  glDeleteProgram(m_shader);
  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
}
