#include "worldgenerator.h"

#include <glm/glm.hpp>

void WorldGenerator::generate(RenderData &renderData) {
  renderData = RenderData{};

  // Global Data
  renderData.globalData = SceneGlobalData{
      .ka = 0.5f,
      .kd = 0.5f,
      .ks = 0.5f,
      .kt = 0.0f // unused
  };

  // Camera Data
  glm::vec3 pos = glm::vec3(3.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  float heightAngle = 30.0f;
  renderData.cameraData = SceneCameraData{
      .pos = glm::vec4(pos, 1.0f),
      .look = -glm::vec4(pos, 0.0f),
      .up = glm::vec4(up, 0.0f),
      .heightAngle = glm::radians(heightAngle),
  };

  // Lights
  SceneLightData directionalLight = SceneLightData{
      .type = LightType::LIGHT_DIRECTIONAL,
      .color = glm::vec4(1.0f),
      .dir = glm::vec4(-3.0f, -2.0f, -1.0f, 0.0f),
  };
  renderData.lights.push_back(directionalLight);

  // Shapes
  glm::mat4 ctm = glm::mat4(1.0f);
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
  renderData.shapes.push_back(cube);
}
