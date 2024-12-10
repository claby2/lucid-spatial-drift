#include "enemymanager.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>
#include <iostream>

EnemyManager::EnemyManager(GLuint shader, std::vector<GLuint>& textures) {
    lastSpawnTime = time(NULL);
    enemyShader = shader;
    enemyTextures = textures;
    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    std::vector<GLfloat> squareVertices = {
        -1.f,  1.f, 0.0f, 0.f, 1.f,
         -1.f, -1.f, 0.0f, 0.f, 0.f,
         1.f, -1.f, 0.0f, 1.f, 0.f,
         1.f,  1.f, 0.0f, 1.f, 1.f,
         -1.f,  1.f, 0.0f, 0.f, 1.f,
         1.f, -1.f, 0.0f, 1.f, 0.f,
    };
    glBufferData(GL_ARRAY_BUFFER, squareVertices.size() * sizeof(GLfloat), squareVertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &squareVAO);
    glBindVertexArray(squareVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void EnemyManager::update(float deltaTime) {
    spawnEnemy();
    for (Enemy& e : enemies) {
        e.update(deltaTime);
    }
}

const int spawnInterval = 10;
const int maxEnemyCount = 10;
void EnemyManager::spawnEnemy() {
    if (time(NULL) - lastSpawnTime > spawnInterval && enemies.size() < maxEnemyCount) {
        enemies.push_back(Enemy(NormalEnemy, cameraPos, 3, 100, this));
        lastSpawnTime = time(NULL);
    }
}

void EnemyManager::drawAllEnemy(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos) {
    this->view = view;
    this->projection = projection;
    this->cameraPos = cameraPos;
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].drawEnemy();
    }
}

void EnemyManager::drawTextureSquare(glm::vec3 pos, glm::vec3 scale, GLuint texture) {
    glUseProgram(enemyShader);
    glBindVertexArray(squareVAO);
    glm::vec3 facing = cameraPos - pos;
    glm::vec3 fromVec = glm::vec3(0, 0, 1);
    glm::vec3 toVec = glm::normalize(facing);
    glm::vec3 axis = glm::cross(fromVec, toVec);
    float angle = glm::acos(glm::dot(fromVec, toVec));
    glm::mat4 rotmat = glm::rotate(glm::mat4(1), angle, axis);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale);
    glm::mat4 translateMat = glm::translate(glm::mat4(1), pos);

    glm::mat4 model = translateMat * rotmat * scaleMat;

    glUniformMatrix4fv(glGetUniformLocation(enemyShader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(enemyShader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(enemyShader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(enemyShader, "sampler"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
