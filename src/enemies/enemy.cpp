#include "enemy.h"
#include "enemymanager.h"
#include "../utils/worldgenerator.h"
#include <set>
#include <tuple>
#include <iostream>


#define SPEED 5.0f
#define TARGET_POS_RAD 10.0f

Enemy::Enemy(EnemyType type, glm::vec3 position) {
  m_type = type;
  m_position = position;
}

glm::vec3 Enemy::getPosition() const { return m_position; }

/*bool recFindPathDFS(int i, int j, int k, int endI, int endJ, int endK,
  std::vector<bool>& worldData, std::vector<glm::vec3>& output, std::set<std::tuple<int, int, int>>& visited) {
  visited.insert(std::make_tuple(i, j, k));
    std::cout << "Visited" << visited.size() << std::endl;
  std::cout << "Output" << output.size() << std::endl;
    if (i == endI && j == endJ && k == endK) {
      output.push_back(glm::vec3(i, j, k));
      return true;
    }
    output.push_back(glm::vec3(i, j, k));
    for (int di = -1; di <= 1; di++) {
      for (int dj = -1; dj <= 1; dj++) {
        for (int dk = -1; dk <= 1; dk++) {
          if (di == 0 && dj == 0 && dk == 0) {
            continue;
          }
          int newI = i + di;
          int newJ = j + dj;
          int newK = k + dk;
          if (newI < 0 || newI >= WORLD_DIMENSION || newJ < 0 || newJ >= WORLD_DIMENSION || newK < 0 || newK >= WORLD_DIMENSION) {
            continue;
          }
          if (visited.find(std::make_tuple(newI, newJ, newK)) != visited.end()) {
            continue;
          }
          if (worldData[newI + newJ * WORLD_DIMENSION + newK * WORLD_DIMENSION * WORLD_DIMENSION]) {
            continue;
          }
          if (recFindPathDFS(newI, newJ, newK, endI, endJ, endK, worldData, output, visited)) {
            return true;
          }
        }
      }
    }
    output.pop_back();
    return false;
}*/

void Enemy::genPath(std::vector<bool>& worldData) {
  int i = rand() % (WORLD_DIMENSION);
  int j = rand() % (WORLD_DIMENSION);
  int k = rand() % (WORLD_DIMENSION);
    while (worldData[i + j * WORLD_DIMENSION + k * WORLD_DIMENSION * WORLD_DIMENSION]) { // find valid end position
      i = rand() % (WORLD_DIMENSION);
      j = rand() % (WORLD_DIMENSION);
      k = rand() % (WORLD_DIMENSION);
    }
    path.clear();
    //std::set<std::tuple<int, int, int>> visited;
    //bool success = recFindPathDFS(currentI, currentJ, currentK, i, j, k, worldData, path, visited);
    //bool success = true;

    // generate a series of path points from current position to i j k on a Bezier curve
    glm::vec3 start = m_position;
    glm::vec3 end = glm::vec3(i, j, k);
    glm::vec3 mid = (start + end) / 2.0f;
    glm::vec3 diff = end - start;
    glm::vec3 control1 = start + glm::vec3(diff.x, 0, 0);
    glm::vec3 control2 = mid + glm::vec3(0, 0, diff.z);
    for (int time = 1; time <= 10; time += 1) {
      float t = time / 10.0f;
      float u = 1.0f - t;
      glm::vec3 point = u * u * u * start + 3 * u * u * t * control1 + 3 * u * t * t * control2 + t * t * t * end;
      path.push_back(point);
    }
}

void Enemy::update(float deltaTime, std::vector<bool>& worldData) {
    if (glm::distance(path.front(), m_position) < 0.1f) {
        path.pop_front();
    }
    if (path.size() == 0) {
        genPath(worldData);
    }
    glm::vec3 targetPosition = path.front();
    path.pop_front();
  if (glm::distance(m_position, targetPosition) < 0.1f) {
    glm::vec3 diff =
        glm::vec3(rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5);
    diff = glm::normalize(diff) * TARGET_POS_RAD;
    targetPosition += diff;
  }
  glm::vec3 velocity = glm::normalize(targetPosition - m_position) * SPEED;
  m_position += velocity * deltaTime;
}
