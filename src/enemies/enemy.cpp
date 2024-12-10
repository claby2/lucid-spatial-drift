#include "enemy.h"
#include "enemymanager.h"
#include <iostream>

glm::vec4 randomColor() {
    return glm::vec4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
}

void printVec(glm::vec4 v) {
    std::cout << v.x << ", " << v.y << ", " << v.z << ", " << v.w << std::endl;
}

const float colorCycleTime = 0.4f;
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
    for (int i = 0; i < 4; i++) {
        printVec(colors[i].currentColor);
    }
}


const float targetPosRad = 10;
Enemy::Enemy(EnemyType t, glm::vec3 startPos, glm::vec3 startSize, float speed, int initHealth, EnemyManager* manager) {
    type = t;
    position = startPos;
    imgSize = startSize;
    this->speed = speed;
    health = initHealth;
    this->manager = manager;
    targetPosition = manager->cameraPos;
    glm::vec3 diff = glm::vec3(rand() % 11 - 5, rand() % 11 - 5, rand() % 11 - 5);
    diff = glm::normalize(diff) * targetPosRad;
    targetPosition += diff;
    velocity = glm::normalize(targetPosition - position) * speed;
    for (int i = 0; i < 4; i++) {
        colors[i].startColor = randomColor();
        colors[i].currentColor = colors[i].startColor;
        colors[i].targetColor = randomColor();
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
    updateColors(deltaTime);
}
