#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <deque>

enum EnemyType { NormalEnemy, SpinEnemy, OctopusEnemy };

struct ColorPair {
public:
  glm::vec4 startColor;
  glm::vec4 currentColor;
  glm::vec4 targetColor;
};

class Enemy {
public:
  Enemy(EnemyType type, glm::vec3 position);

  glm::vec3 getPosition() const;

  void update(float deltaTime, std::vector<bool>& worldData);
  std::deque<glm::vec3> path;
private:
  EnemyType m_type;
  glm::vec3 m_position;
  void genPath(std::vector<bool>& worldData);
};
