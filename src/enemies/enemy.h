#pragma once
#include "utils/worldgenerator.h"
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <optional>

enum EnemyType { NormalEnemy, SpinEnemy, OctopusEnemy };

struct ColorPair {
public:
    glm::vec4 startColor;
    glm::vec4 currentColor;
    glm::vec4 targetColor;
};

class Coordinate {
public:
  int x;
  int y;
  int z;

  Coordinate() : x(0), y(0), z(0) {}

  Coordinate(int x, int y, int z) : x(x), y(y), z(z) {}

  bool operator==(const Coordinate &other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  bool inBounds() const {
    return x >= 0 && y >= 0 && z >= 0 && x < WORLD_DIMENSION &&
           y < WORLD_DIMENSION && z < WORLD_DIMENSION;
  }
};

class Enemy {
public:
  Enemy(EnemyType type, glm::vec3 position);

  glm::vec3 getPosition() const;
  std::array<ColorPair,4> colors;
  void updateColors(float deltaTime);

  void update(float deltaTime, const std::vector<bool> &worldData);

private:
  EnemyType m_type;
  glm::vec3 m_position;

  std::optional<Coordinate> m_targetPosition;
};
