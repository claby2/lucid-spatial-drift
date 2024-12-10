#include "projectile.h"
#include "utils/scenedata.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

void Projectile::update(float deltaTime) {
  m_position += m_direction * deltaTime;
}

glm::vec3 Projectile::getPosition() const { return m_position; }

void ProjectileManager::spawnProjectile(glm::vec3 position,
                                        glm::vec3 direction) {
  m_projectiles.push_back(Projectile(position, direction));
}

void ProjectileManager::update(float deltaTime) {
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
