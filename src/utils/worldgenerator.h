#pragma once

#include "objectmanager.h"
#include "sceneparser.h"

const int WORLD_DIMENSION = 96;

class WorldGenerator {
public:
  WorldGenerator() = default;

  void initialize();

  void generate();
  RenderShapeData getShapeData() const;
  void installVbo();
  void bindVao();
  void unbindVao();
  int getVertexCount() const;
  std::vector<bool>& getWorldData() { return WorldGenerator::m_worldData; };
  void setWorldData(std::vector<bool> d) {WorldGenerator::m_worldData = d;};

  ~WorldGenerator();

private:
  void addVec3(glm::vec3 v);
  void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                glm::vec3 bottomRight);
  void makeFace(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                glm::vec3 bottomRight);
  void addCube(glm::vec3 delta);

  VaoManager m_vao;
  VboManager m_vbo;

  std::vector<float> m_vertexData;

  std::vector<bool> m_worldData;
};
