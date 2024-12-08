#include "skybox.h"
#include "shaderloader.h"
#include <QImageReader>
#include <glm/glm.hpp>

void initializeImage(GLenum cubeFace, QImage &image, const QString &path) {
  image = QImage(path);
  image = image.convertToFormat(QImage::Format_RGBA8888).mirrored();
  glTexImage2D(cubeFace, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image.bits());
}

// faces is a vector of filepaths to the faces of the skybox
void Skybox::initialize() {
  m_shader = ShaderLoader::createShaderProgram(
      ":/resources/shaders/skybox.vert", ":/resources/shaders/skybox.frag");

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

  initializeImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_image1,
                  ":/resources/skyboxImages/px.png");
  initializeImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_image2,
                  ":/resources/skyboxImages/nx.png");
  initializeImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_image3,
                  ":/resources/skyboxImages/py.png");
  initializeImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_image4,
                  ":/resources/skyboxImages/ny.png");
  initializeImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_image5,
                  ":/resources/skyboxImages/pz.png");
  initializeImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_image6,
                  ":/resources/skyboxImages/nz.png");

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  glUseProgram(m_shader);
  glUniform1i(glGetUniformLocation(m_shader, "skybox"), 0);
  glUseProgram(0);

  std::vector<float> skyboxVertices = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(GLfloat),
               skyboxVertices.data(), GL_STATIC_DRAW);
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::render(const Camera &camera) {
  glDepthMask(GL_FALSE);
  glUseProgram(m_shader);

  glUniformMatrix4fv(glGetUniformLocation(m_shader, "projection"), 1, GL_FALSE,
                     &camera.getProjection()[0][0]);
  glm::mat4 nonTranslateView = glm::mat4(glm::mat3(camera.getView()));
  glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, GL_FALSE,
                     &nonTranslateView[0][0]);

  glBindVertexArray(m_vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

Skybox::~Skybox() {
  glDeleteProgram(m_shader);
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
  glDeleteTextures(1, &m_texture);
}
