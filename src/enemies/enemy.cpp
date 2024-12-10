#include "enemy.h"
#include "enemymanager.h"
#include <iostream>

const float targetPosRad = 3;
Enemy::Enemy(EnemyType t, glm::vec3 startPos, float speed, int initHealth, EnemyManager* manager) {
    type = t;
    position = startPos;
    position[2] -= 1;
    this->speed = speed;
    health = initHealth;
    this->manager = manager;
    targetPosition = manager->cameraPos;
    glm::vec3 diff = glm::vec3(rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5);
    diff = glm::normalize(diff) * targetPosRad;
    targetPosition += diff;
    velocity = glm::normalize(targetPosition - position) * speed;
}

void Enemy::drawEnemy() {
    if (type == NormalEnemy) {
        manager->drawTextureSquare(position, glm::vec3(0.1, 0.1, 0.1), manager->enemyTextures[0]);
    }
    else if (type == SpinEnemy) {
        manager->drawTextureSquare(position, glm::vec3(0.1, 0.1, 0.1), manager->enemyTextures[0]);
    }
    else if (type == OctopusEnemy) {
        manager->drawTextureSquare(position, glm::vec3(0.1, 0.1, 0.1), manager->enemyTextures[0]);
    }
}


void Enemy::update(float deltaTime) {
    if (glm::distance(position, targetPosition) < 0.1f) {
        targetPosition = manager->cameraPos;
        glm::vec3 diff = glm::vec3(rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5);
        diff = glm::normalize(diff) * targetPosRad;
        targetPosition += diff;
        velocity = glm::normalize(targetPosition - position) * speed;
    }
    auto posChange = velocity * deltaTime;
    this->position += posChange;
}
