#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "enemy.h"

class EnemyManager
{
public:
    EnemyManager(){};
    EnemyManager(GLuint shader, std::vector<GLuint>& textures);
    std::vector<Enemy> enemies;
    int lastSpawnTime;
    void spawnEnemy();
    void drawAllEnemy();
    GLuint squareVBO;
    GLuint squareVAO;
    GLuint enemyShader;
    void drawTextureSquare(glm::vec3 pos, glm::vec3 scale, GLuint texture, std::array<ColorPair, 4> colors);
    void drawAllEnemy(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos);
    void update(float deltaTime);
    void render(Enemy& e);

    glm::vec3 cameraPos;
    glm::mat4 view;
    glm::mat4 projection;

    std::vector<GLuint> enemyTextures;
};

