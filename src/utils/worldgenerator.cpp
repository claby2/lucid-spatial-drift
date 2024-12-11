#include "worldgenerator.h"

#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#define WORLD_DIMENSION 72
#define ITERATIONS 20

int countNeighbors(const std::vector<bool> &worldData, int x, int y, int z) {
  int count = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      for (int dz = -1; dz <= 1; dz++) {
        if (dx == 0 && dy == 0 && dz == 0)
          continue;
        int nx = x + dx;
        int ny = y + dy;
        int nz = z + dz;
        if (nx < 0 || nx >= WORLD_DIMENSION || ny < 0 ||
            ny >= WORLD_DIMENSION || nz < 0 || nz >= WORLD_DIMENSION)
          continue;
        if (worldData[nx + ny * WORLD_DIMENSION +
                      nz * WORLD_DIMENSION * WORLD_DIMENSION])
          count++;
      }
    }
  }
  return count;
}

// Generate a world of size WORLD_DIMENSION x WORLD_DIMENSION x WORLD_DIMENSION
// True means there is a block, false means there is not
std::vector<bool> generateWorldData() {
  // TODO: Make this 3D Game of Life
  std::vector<bool> res;

  std::srand(std::time(0));

  // Randomly generate world
  for (int x = 0; x < WORLD_DIMENSION; x++) {
    for (int y = 0; y < WORLD_DIMENSION; y++) {
      for (int z = 0; z < WORLD_DIMENSION; z++) {
        res.push_back(std::rand() % 2 == 0);
      }
    }
  }

  for (int i = 0; i < ITERATIONS; i++) {
    std::vector<bool> newRes = res;
    for (int x = 0; x < WORLD_DIMENSION; x++) {
      for (int y = 0; y < WORLD_DIMENSION; y++) {
        for (int z = 0; z < WORLD_DIMENSION; z++) {
          int neighbors = countNeighbors(res, x, y, z);
          int index =
              x + y * WORLD_DIMENSION + z * WORLD_DIMENSION * WORLD_DIMENSION;
          if (res[index]) {
            if (neighbors < 13) {
              newRes[index] = false;
            }
          } else {
            if (neighbors >= 14 && neighbors <= 19) {
              newRes[index] = true;
            }
          }
        }
      }
    }
    res = newRes;
  }

  return res;
}

void WorldGenerator::addVec3(glm::vec3 v) {
  m_vertexData.push_back(v.x);
  m_vertexData.push_back(v.y);
  m_vertexData.push_back(v.z);
}

void WorldGenerator::makeTile(glm::vec3 topLeft, glm::vec3 topRight,
                              glm::vec3 bottomLeft, glm::vec3 bottomRight) {
  addVec3(topLeft);
  addVec3(
      glm::normalize(glm::cross(bottomLeft - topLeft, bottomRight - topLeft)));
  addVec3(bottomLeft);
  addVec3(glm::normalize(
      glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft)));
  addVec3(bottomRight);
  addVec3(glm::normalize(
      glm::cross(topLeft - bottomRight, bottomLeft - bottomRight)));

  addVec3(topLeft);
  addVec3(
      glm::normalize(glm::cross(bottomRight - topLeft, topRight - topLeft)));
  addVec3(bottomRight);
  addVec3(glm::normalize(
      glm::cross(topRight - bottomRight, topLeft - bottomRight)));
  addVec3(topRight);
  addVec3(
      glm::normalize(glm::cross(topLeft - topRight, bottomRight - topRight)));
}

void WorldGenerator::makeFace(glm::vec3 topLeft, glm::vec3 topRight,
                              glm::vec3 bottomLeft, glm::vec3 bottomRight) {
  const float m_param1 = 1.0f;
  float size = 1.0f / m_param1;

  glm::vec3 colDelta = (topRight - topLeft) * size;
  glm::vec3 rowDelta = (bottomLeft - topLeft) * size;

  for (int col = 0; col < m_param1; col++) {
    for (int row = 0; row < m_param1; row++) {
      glm::vec3 tl = topLeft + (float)col * colDelta + (float)row * rowDelta;
      glm::vec3 tr = tl + colDelta;
      glm::vec3 bl = tl + rowDelta;
      glm::vec3 br = bl + colDelta;

      makeTile(tl, tr, bl, br);
    }
  }
}

void WorldGenerator::addCube(glm::vec3 delta) {
  makeFace(glm::vec3(-0.5f, 0.5f, 0.5f) + delta,
           glm::vec3(0.5f, 0.5f, 0.5f) + delta,
           glm::vec3(-0.5f, -0.5f, 0.5f) + delta,
           glm::vec3(0.5f, -0.5f, 0.5f) + delta);

  makeFace(glm::vec3(-0.5f, 0.5f, -0.5f) + delta,
           glm::vec3(-0.5f, 0.5f, 0.5f) + delta,
           glm::vec3(-0.5f, -0.5f, -0.5f) + delta,
           glm::vec3(-0.5f, -0.5f, 0.5f) + delta);

  makeFace(glm::vec3(0.5f, 0.5f, 0.5f) + delta,
           glm::vec3(0.5f, 0.5f, -0.5f) + delta,
           glm::vec3(0.5f, -0.5f, 0.5f) + delta,
           glm::vec3(0.5f, -0.5f, -0.5f) + delta);

  makeFace(glm::vec3(0.5f, 0.5f, -0.5f) + delta,
           glm::vec3(-0.5f, 0.5f, -0.5f) + delta,
           glm::vec3(0.5f, -0.5f, -0.5f) + delta,
           glm::vec3(-0.5f, -0.5f, -0.5f) + delta);

  makeFace(glm::vec3(0.5f, 0.5f, 0.5f) + delta,
           glm::vec3(-0.5f, 0.5f, 0.5f) + delta,
           glm::vec3(0.5f, 0.5f, -0.5f) + delta,
           glm::vec3(-0.5f, 0.5f, -0.5f) + delta);

  makeFace(glm::vec3(0.5f, -0.5f, -0.5f) + delta,
           glm::vec3(-0.5f, -0.5f, -0.5f) + delta,
           glm::vec3(0.5f, -0.5f, 0.5f) + delta,
           glm::vec3(-0.5f, -0.5f, 0.5f) + delta);
}

void WorldGenerator::initialize() {
  m_vao.create();
  m_vbo.create();
}

// Generate vertex data
void WorldGenerator::generate() {
  worldData = generateWorldData();

  for (int i = 0; i < WORLD_DIMENSION; i++) {
    for (int j = 0; j < WORLD_DIMENSION; j++) {
      for (int k = 0; k < WORLD_DIMENSION; k++) {
        if (!worldData[i + j * WORLD_DIMENSION +
                       k * WORLD_DIMENSION * WORLD_DIMENSION])
          continue;
        glm::vec3 delta = glm::vec3(i, j, k);

        addCube(delta);
      }
    }
  }
}

RenderShapeData WorldGenerator::getShapeData() const {
  glm::mat4 identity = glm::mat4(1.0f);
  RenderShapeData shapeData =
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
                      .ctm = identity};
  return shapeData;
}

void WorldGenerator::installVbo() {
  m_vao.bind();
  m_vbo.bind();
  m_vbo.allocate(m_vertexData);
  m_vao.enable();
  m_vbo.unbind();
  m_vao.unbind();
}

void WorldGenerator::bindVao() { m_vao.bind(); }
void WorldGenerator::unbindVao() { m_vao.unbind(); }

int WorldGenerator::getVertexCount() const { return m_vertexData.size() / 6; }

WorldGenerator::~WorldGenerator() {
  m_vbo.free();
  m_vao.free();
}
