#pragma once
#include "enemy.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class EnemyManager {
public:
  EnemyManager() = default;
  EnemyManager(GLuint shader, const std::vector<GLuint> &enemyTextures);

  void update(float deltaTime, std::vector<bool>* worldData);
  void render(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection);

private:
  void drawTextureSquare(glm::vec3 enemyPosition, GLuint texture,
                         glm::vec3 cameraPos, glm::mat4 view,
                         glm::mat4 projection);
  void spawnEnemy();

  int m_lastSpawnTime;
  std::vector<GLuint> m_enemyTextures;

  GLuint m_squareVBO;
  GLuint m_squareVAO;
  GLuint m_shader;

  std::vector<Enemy> m_enemies;

  std::vector<bool>* m_worldData;
};
