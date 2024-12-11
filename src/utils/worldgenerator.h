#pragma once

#include "objectmanager.h"
#include "sceneparser.h"

class WorldGenerator {
public:
  WorldGenerator() = default;

  void initialize();

  void generate();
  RenderShapeData getShapeData() const;
  std::vector<float>& getVertexData() { return m_vertexData; };
  void installVbo();
  void bindVao();
  void unbindVao();
  int getVertexCount() const;

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
};
