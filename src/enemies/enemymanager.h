#pragma once
#include <glm/glm.hpp>
#include <vector>

enum EnemyType {
    NormalEnemy,
    SpinEnemy,
    OctopusEnemy
};

class Enemy {
public:
    Enemy(EnemyType t, glm::vec3 startPos, float speed, int initHealth);
    glm::vec3 position;
    EnemyType type;
    glm::vec3 targetPosition;
    float speed;
    int health;
    void drawEnemy();
    void checkHit(glm::vec3 rayDir, glm::vec3 rayEye)
};

class EnemyManager
{
public:
    EnemyManager();
    std::vector<Enemy> enemies;
    float lastSpawnTime;
    void spawnEnemy();
    void drawAllEnemy();
    GLuint squareVBO;
    GLuint squareVAO;
    GLuint enemyShader;
    void drawTextureSquare(glm::vec3 pos, glm::vec3 scale, GLuint texture);

    glm::vec3 cameraPos;
    glm::mat4 view;
    glm::mat4 projection;
};

