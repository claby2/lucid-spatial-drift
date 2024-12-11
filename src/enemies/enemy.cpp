#include "enemy.h"
#include "enemymanager.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#define SPEED 10.0f
#define TARGET_POS_RAD 1.0f

glm::vec4 randomColor() {
    return glm::vec4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
}

Enemy::Enemy(EnemyType type, glm::vec3 position) {
  m_type = type;
  m_position = position;
  for (int i = 0; i < 4; i++) {
      colors[i].startColor = randomColor();
      colors[i].currentColor = colors[i].startColor;
      colors[i].targetColor = randomColor();
  }
}

glm::vec3 Enemy::getPosition() const { return m_position; }

Coordinate randomEmptyCoordinate(const std::vector<bool> &worldData) {
  while (true) {
    int x = rand() % WORLD_DIMENSION;
    int y = rand() % WORLD_DIMENSION;
    int z = rand() % WORLD_DIMENSION;
    if (!worldData[x + y * WORLD_DIMENSION +
                   z * WORLD_DIMENSION * WORLD_DIMENSION]) {
      return Coordinate(x, y, z);
    }
  }
}

void printVec(glm::vec4 v) {
    std::cout << v.x << ", " << v.y << ", " << v.z << ", " << v.w << std::endl;
}

const float colorCycleTime = 0.8f;
void Enemy::updateColors(float deltaTime) {
    if (glm::distance(colors[0].currentColor, colors[0].targetColor) < 0.1f) {
        for (int i = 0; i < 4; i++) {
            colors[i].startColor = colors[i].targetColor;
            colors[i].targetColor = randomColor();
        }
    }
    for (int i = 0; i < 4; i++) {
        printVec(colors[i].currentColor);
    }
    for (int i = 0; i < 4; i++) {
        colors[i].currentColor += (colors[i].targetColor - colors[i].startColor) / colorCycleTime * deltaTime;
    }
    if (abs(colors[0].currentColor[0]) > 1) {
        colors[0].currentColor = colors[0].targetColor;
    }
    for (int i = 0; i < 4; i++) {
        printVec(colors[i].currentColor);
    }
}

// Returns true if there are no blocks between start and end
bool noBlocksBetween(const Coordinate &start, const Coordinate &end,
                     const std::vector<bool> &worldData) {
  int dx = abs(end.x - start.x);
  int dy = abs(end.y - start.y);
  int dz = abs(end.z - start.z);

  int steps = std::max({dx, dy, dz});
  if (steps == 0) {
    int idx = start.x + start.y * WORLD_DIMENSION +
              start.z * WORLD_DIMENSION * WORLD_DIMENSION;
    return !worldData[idx];
  }

  float sx = static_cast<float>(end.x - start.x) / steps;
  float sy = static_cast<float>(end.y - start.y) / steps;
  float sz = static_cast<float>(end.z - start.z) / steps;

  float x = static_cast<float>(start.x);
  float y = static_cast<float>(start.y);
  float z = static_cast<float>(start.z);

  for (int i = 0; i <= steps; ++i) {
    int ix = static_cast<int>(std::round(x));
    int iy = static_cast<int>(std::round(y));
    int iz = static_cast<int>(std::round(z));
    if (ix < 0 || ix >= WORLD_DIMENSION || iy < 0 || iy >= WORLD_DIMENSION ||
        iz < 0 || iz >= WORLD_DIMENSION) {
      return false;
    }
    int idx =
        ix + iy * WORLD_DIMENSION + iz * WORLD_DIMENSION * WORLD_DIMENSION;
    if (worldData[idx]) {
      return false;
    }
    x += sx;
    y += sy;
    z += sz;
  }
  return true;
}

void Enemy::update(float deltaTime, const std::vector<bool> &worldData) {
  if (worldData.size() != WORLD_DIMENSION * WORLD_DIMENSION * WORLD_DIMENSION) {
    std::cout << "World data size is not correct" << std::endl;
    return;
  }

  Coordinate currentPos(static_cast<int>(m_position.x),
                        static_cast<int>(m_position.y),
                        static_cast<int>(m_position.z));

  if (!m_targetPosition.has_value() ||
      glm::distance(glm::vec3(m_targetPosition.value().x,
                              m_targetPosition.value().y,
                              m_targetPosition.value().z),
                    m_position) < TARGET_POS_RAD) {
    Coordinate target = randomEmptyCoordinate(worldData);
    if (noBlocksBetween(currentPos, target, worldData)) {
      m_targetPosition = target;
    }
    return;
  }

  m_position += SPEED * deltaTime *
                glm::normalize(glm::vec3(m_targetPosition.value().x,
                                         m_targetPosition.value().y,
                                         m_targetPosition.value().z) -
                               m_position);
  updateColors(deltaTime);
}
