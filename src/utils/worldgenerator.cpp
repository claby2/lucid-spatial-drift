#include "worldgenerator.h"

#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define WORLD_DIMENSION 15

std::vector<bool> generateWorldData() {
  std::vector<bool> res;

  std::srand(std::time(0));

  int randomNum = std::rand() % 2;
  for (int i = 0; i < WORLD_DIMENSION; i++) {
    for (int j = 0; j < WORLD_DIMENSION; j++) {
      for (int k = 0; k < WORLD_DIMENSION; k++) {
        res.push_back(std::rand() % 2 == 0);
      }
    }
  }
  return res;
}

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
  std::vector<bool> worldData = generateWorldData();

  glm::mat4 identity = glm::mat4(1.0f);
  for (int i = 0; i < WORLD_DIMENSION; i++) {
    for (int j = 0; j < WORLD_DIMENSION; j++) {
      for (int k = 0; k < WORLD_DIMENSION; k++) {
        if (!worldData[i + j * WORLD_DIMENSION +
                       k * WORLD_DIMENSION * WORLD_DIMENSION])
          continue;
        glm::mat4 ctm = glm::translate(
            identity, glm::vec3(static_cast<float>(i), static_cast<float>(j),
                                static_cast<float>(k)));
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
    }
  }
}
