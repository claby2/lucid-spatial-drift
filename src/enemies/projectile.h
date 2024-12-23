#pragma once
#include "utils/sceneparser.h"

class Projectile {
public:
  Projectile(glm::vec3 position, glm::vec3 direction)
      : m_position(position), m_direction(direction) {}

  glm::vec3 getPosition() const;

  void update(float deltaTime);

private:
  glm::vec3 m_position;
  glm::vec3 m_direction;
};

class ProjectileManager {
public:
  ProjectileManager() = default;

  void spawnProjectile(glm::vec3 position, glm::vec3 direction);
  void update(float deltaTime, glm::vec3 playerPos);
  std::vector<RenderShapeData> getRenderData() const;
  std::vector<glm::vec3> getProjectilePositions() const;

private:
  std::vector<Projectile> m_projectiles;
};
