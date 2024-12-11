#pragma once
#include "utils/worldgenerator.h"
#include <glm/glm.hpp>
#include <vector>

enum EnemyType { NormalEnemy, SpinEnemy, OctopusEnemy };

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

  struct Hash {
    size_t operator()(const Coordinate &c) const {
      return std::hash<int>()(c.x) ^ std::hash<int>()(c.y) ^
             std::hash<int>()(c.z);
    }
  };
};

class Enemy {
public:
  Enemy(EnemyType type, glm::vec3 position);

  glm::vec3 getPosition() const;

  void update(float deltaTime, const std::vector<bool> &worldData);

private:
  EnemyType m_type;
  glm::vec3 m_position;

  std::optional<Coordinate> m_targetPosition;
};
