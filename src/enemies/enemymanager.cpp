#include "enemymanager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../utils/worldgenerator.h"
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

void EnemyManager::update(float deltaTime, std::vector<bool>* worldData) {
    m_worldData = worldData;
  spawnEnemy();
  for (Enemy &e : m_enemies) {
    e.update(deltaTime, *worldData);
  }
}

glm::vec3 findRandomUnoccupiedPosition(std::vector<bool>& worldData) {
  int i = rand() % WORLD_DIMENSION;
  int j = rand() % WORLD_DIMENSION;
  int k = rand() % WORLD_DIMENSION;
  while (worldData[i + j * WORLD_DIMENSION + k * WORLD_DIMENSION * WORLD_DIMENSION]) {
    i = rand() % WORLD_DIMENSION;
    j = rand() % WORLD_DIMENSION;
    k = rand() % WORLD_DIMENSION;
  }
  return glm::vec3(i, j, k);
}

void EnemyManager::spawnEnemy() {
  if (time(NULL) - m_lastSpawnTime > SPAWN_INTERVAL &&
      m_enemies.size() < MAX_ENEMY_COUNT) {
    m_enemies.push_back(Enemy(NormalEnemy, findRandomUnoccupiedPosition(*m_worldData)));
    m_lastSpawnTime = time(NULL);
  }
}

void EnemyManager::drawTextureSquare(glm::vec3 enemyPosition, GLuint texture,
                                     glm::vec3 cameraPos, glm::mat4 view,
                                     glm::mat4 projection) {
  // Just make scale a constant for now
  const glm::vec3 scale = glm::vec3(0.1, 0.1, 0.1);

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
                      projection);
  }
}
