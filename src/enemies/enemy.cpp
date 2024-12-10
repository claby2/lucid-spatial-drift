#include "enemy.h"
#include "enemymanager.h"

#define SPEED 10.0f
#define TARGET_POS_RAD 10.0f

Enemy::Enemy(EnemyType type, glm::vec3 position) {
  m_type = type;
  m_position = position;
}

glm::vec3 Enemy::getPosition() const { return m_position; }

void Enemy::update(float deltaTime, glm::vec3 targetPosition) {
  if (glm::distance(m_position, targetPosition) < 0.1f) {
    glm::vec3 diff =
        glm::vec3(rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5);
    diff = glm::normalize(diff) * TARGET_POS_RAD;
    targetPosition += diff;
  }
  glm::vec3 velocity = glm::normalize(targetPosition - m_position) * SPEED;
  m_position += velocity * deltaTime;
}
