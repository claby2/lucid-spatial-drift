#include "projectile.h"
#include "utils/scenedata.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define DESPAWN_DISTANCE 100.f

void Projectile::update(float deltaTime) {
  m_position += m_direction * deltaTime;
}

glm::vec3 Projectile::getPosition() const { return m_position; }

void ProjectileManager::spawnProjectile(glm::vec3 position,
                                        glm::vec3 direction) {
  m_projectiles.push_back(Projectile(position, direction));
}

void ProjectileManager::update(float deltaTime, glm::vec3 playerPos) {
  // Remove all projectiles that are too far away
  m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(),
                                     [playerPos](const Projectile &p) {
                                       return glm::distance(p.getPosition(),
                                                            playerPos) >
                                              DESPAWN_DISTANCE;
                                     }),
                      m_projectiles.end());

  for (Projectile &p : m_projectiles) {
    p.update(deltaTime);
  }
}

std::vector<RenderShapeData> ProjectileManager::getRenderData() const {

  std::vector<RenderShapeData> renderData;
  const glm::mat4 identity = glm::mat4(1.0f);
  for (const Projectile &p : m_projectiles) {

    glm::mat4 ctm = glm::translate(identity, p.getPosition());
    RenderShapeData cube =
        RenderShapeData{.primitive =
                            ScenePrimitive{
                                .type = PrimitiveType::PRIMITIVE_CUBE,
                                .material =
                                    SceneMaterial{
                                        .cAmbient = glm::vec4(1.0f),
                                        .cDiffuse = glm::vec4(1.0f),
                                        .cSpecular = glm::vec4(1.0f),
                                        .shininess = 25.0f,
                                        .cReflective = glm::vec4(0.0f),
                                    },
                            },
                        .ctm = ctm};
    renderData.push_back(cube);
  }
  return renderData;
}

std::vector<glm::vec3> ProjectileManager::getProjectilePositions() const {
  std::vector<glm::vec3> positions;
  for (const Projectile &p : m_projectiles) {
    positions.push_back(p.getPosition());
  }
  return positions;
}
