#pragma once
#include "enemy.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

int m_score = 0; // Global score variable

class EnemyManager {
public:
  EnemyManager() = default;
  EnemyManager(GLuint shader, const std::vector<GLuint> &enemyTextures);

  void update(float deltaTime, const std::vector<bool> &worldData,
              const std::vector<glm::vec3> &projectilePositions);
  void render(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection);

private:
  void drawTextureSquare(glm::vec3 enemyPosition, GLuint texture,
                         glm::vec3 cameraPos, glm::mat4 view,
                         glm::mat4 projection, std::array<ColorPair, 4> colors);
  void spawnEnemy();

  int m_lastSpawnTime;
  std::vector<GLuint> m_enemyTextures;

  GLuint m_squareVBO;
  GLuint m_squareVAO;
  GLuint m_shader;

  std::vector<Enemy> m_enemies;
};
