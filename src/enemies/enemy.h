#pragma once
#include <glm/glm.hpp>
enum EnemyType {
    NormalEnemy,
    SpinEnemy,
    OctopusEnemy
};

class EnemyManager;

class Enemy {
public:
    Enemy(EnemyType t, glm::vec3 startPos, float speed, int initHealth, EnemyManager* manager);
    glm::vec3 position;
    EnemyType type;
    glm::vec3 targetPosition;
    glm::vec3 velocity;
    float speed;
    int health;
    void drawEnemy();
    void checkHit(glm::vec3 rayDir, glm::vec3 rayEye);
    void update(float deltaTime);
    EnemyManager* manager;
};


