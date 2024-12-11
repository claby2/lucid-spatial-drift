#include "enemymanager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>
#include <iostream>

#define SPAWN_INTERVAL 0.1
#define MAX_ENEMY_COUNT 100

EnemyManager::EnemyManager(GLuint shader,
                           const std::vector<GLuint> &enemyTextures) {
  // TODO: Creating the shader program and enemy textures should ideally be
  //       done here, but for now we'll just pass them in
  m_shader = shader;
  m_enemyTextures = enemyTextures;

  glUseProgram(m_shader);

  m_lastSpawnTime = time(NULL);

  glGenBuffers(1, &m_squareVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_squareVBO);
  std::vector<GLfloat> squareVertices = {
      -1.f, 1.f,  0.0f, 0.f, 1.f, -1.f, -1.f, 0.0f, 0.f, 0.f,
      1.f,  -1.f, 0.0f, 1.f, 0.f, 1.f,  1.f,  0.0f, 1.f, 1.f,
      -1.f, 1.f,  0.0f, 0.f, 1.f, 1.f,  -1.f, 0.0f, 1.f, 0.f,
  };
  glBufferData(GL_ARRAY_BUFFER, squareVertices.size() * sizeof(GLfloat),
               squareVertices.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &m_squareVAO);
  glBindVertexArray(m_squareVAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void EnemyManager::update(float deltaTime, const std::vector<bool> &worldData,
                          const std::vector<glm::vec3> &projectilePositions) {
  // Prune enemies that are too close to any projectile
  m_enemies.erase(
      std::remove_if(m_enemies.begin(), m_enemies.end(),
                     [projectilePositions](const Enemy &e) {
                       for (const glm::vec3 &p : projectilePositions) {
                         if (glm::distance(e.getPosition(), p) < 1.0f) {
                           return true;
                         }
                       }
                       return false;
                     }),
      m_enemies.end());

  spawnEnemy();
  for (Enemy &e : m_enemies) {
    e.update(deltaTime, worldData);
  }
}

void EnemyManager::spawnEnemy() {
  if (time(NULL) - m_lastSpawnTime > SPAWN_INTERVAL &&
      m_enemies.size() < MAX_ENEMY_COUNT) {
    m_enemies.push_back(
        Enemy(NormalEnemy,
              glm::vec3(rand() % WORLD_DIMENSION, rand() % WORLD_DIMENSION,
                        rand() % WORLD_DIMENSION)));
    m_lastSpawnTime = time(NULL);
  }
}

void EnemyManager::drawTextureSquare(glm::vec3 enemyPosition, GLuint texture,
                                     glm::vec3 cameraPos, glm::mat4 view,
                                     glm::mat4 projection, std::array<ColorPair, 4> colors) {
  // Just make scale a constant for now
  const glm::vec3 scale = glm::vec3(2.0f, 2.0f, 2.0f);

  glUseProgram(m_shader);
  glBindVertexArray(m_squareVAO);

  glm::vec3 facing = cameraPos - enemyPosition;
  glm::vec3 fromVec = glm::vec3(0, 0, 1);
  glm::vec3 toVec = glm::normalize(facing);
  glm::vec3 axis = glm::cross(fromVec, toVec);
  float angle = glm::acos(glm::dot(fromVec, toVec));
  glm::mat4 rotmat = glm::rotate(glm::mat4(1), angle, axis);
  glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale);
  glm::mat4 translateMat = glm::translate(glm::mat4(1), enemyPosition);

  glm::mat4 model = translateMat * rotmat * scaleMat;
  glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE,
                     glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE,
                     glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(m_shader, "projectionMatrix"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  glUniform4fv(glGetUniformLocation(m_shader, "topLeftColor"), 1, &(colors[0].currentColor[0]));
  glUniform4fv(glGetUniformLocation(m_shader, "topRightColor"), 1, &(colors[1].currentColor[0]));
  glUniform4fv(glGetUniformLocation(m_shader, "bottomLeftColor"), 1, &(colors[2].currentColor[0]));
  glUniform4fv(glGetUniformLocation(m_shader, "bottomRightColor"), 1, &(colors[3].currentColor[0]));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(glGetUniformLocation(m_shader, "sampler"), 0);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

void EnemyManager::render(glm::vec3 cameraPos, glm::mat4 view,
                          glm::mat4 projection) {
  for (const Enemy &e : m_enemies) {
    drawTextureSquare(e.getPosition(), m_enemyTextures[0], cameraPos, view,
                      projection, e.colors);
  }
}
