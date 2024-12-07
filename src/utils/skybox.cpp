#include "skybox.h"
#include "shaderloader.h"
#include <QImageReader>

// faces is a vector of filepaths to the faces of the skybox
void Skybox::initialize() {
    m_shader = ShaderLoader::createShaderProgram(
        ":/resources/shaders/skybox.vert", ":/resources/shaders/skybox.frag");

    glGenTextures(1, &m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    QString posx = QString(":/resources/skyboxImages/px.png");
    m_image1 = QImage(posx);
    m_image1 = m_image1.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, m_image1.width(), m_image1.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image1.bits());
    QString negx = QString(":/resources/skyboxImages/nx.png");
    m_image2 = QImage(negx);
    m_image2 = m_image2.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, m_image2.width(), m_image2.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image2.bits());
    QString posy = QString(":/resources/skyboxImages/py.png");
    m_image3 = QImage(posy);
    m_image3 = m_image3.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, m_image3.width(), m_image3.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image3.bits());
    QString negy = QString(":/resources/skyboxImages/ny.png");
    m_image4 = QImage(negy);
    m_image4 = m_image4.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, m_image4.width(), m_image4.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image4.bits());
    QString posz = QString(":/resources/skyboxImages/pz.png");
    m_image5 = QImage(posz);
    m_image5 = m_image5.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, m_image5.width(), m_image5.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image5.bits());
    QString negz = QString(":/resources/skyboxImages/nz.png");
    m_image6 = QImage(negz);
    m_image6 = m_image6.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, m_image6.width(), m_image6.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image6.bits());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, "skybox"), 0);
    glUseProgram(0);

    std::vector<float> skyboxVertices = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size()*sizeof(GLfloat), skyboxVertices.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          nullptr);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Skybox::~Skybox() {
    glDeleteProgram(m_shader);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_texture);
}
