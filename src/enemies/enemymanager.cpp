#include "enemymanager.h"

EnemyManager::EnemyManager() {
    lastSpawnTime = time(NULL);
    enemyShader = LoadShaders("resources/shaders/enemy.vert", "resources/shaders/enemy.frag");
    
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


const int spawnInterval = 10000;
void EnemyManager::spawnEnemy() {
    if (time(NULL) - lastSpawnTime > spawnInterval) {
        enemies.push_back(Enemy(NormalEnemy, glm::vec3(0, 0, 0), 0.1, 10));
        lastSpawnTime = time(NULL);
    }
}

void EnemyManager::drawAllEnemy() {
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
    glm::mat4 rotmat = glm::rotate(angle, axis);
    glm::mat4 scaleMat = glm::scale(scale);
    glm::mat4 translateMat = glm::translate(pos);

    glm::mat4 model = translateMat * rotmat * scaleMat;

    glUniformMatrix4fv(glGetUniformLocation(enemyShader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(enemyShader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(enemyShader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(enemyShader, "ourTexture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}