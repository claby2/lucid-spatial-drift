#pragma once
#include <glm/glm.hpp>
#include <array>
enum EnemyType {
    NormalEnemy,
    SpinEnemy,
    OctopusEnemy
};

class EnemyManager;

struct ColorPair {
    public:
    glm::vec4 startColor;
    glm::vec4 currentColor;
    glm::vec4 targetColor;
};

class Enemy {
public:
    Enemy(EnemyType t, glm::vec3 startPos, glm::vec3 startSize, float speed, int initHealth, EnemyManager* manager);
    glm::vec3 position;
    glm::vec3 imgSize;
    EnemyType type;
    glm::vec3 targetPosition;
    glm::vec3 velocity;
    std::array<ColorPair,4> colors;
    float speed;
    int health;
    void drawEnemy();
    void checkHit(glm::vec3 rayDir, glm::vec3 rayEye);
    void update(float deltaTime);
    void updateColors(float deltaTime);
    EnemyManager* manager;
};


