#include "realtime.h"

#include "settings.h"
#include "utils/debug.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include "utils/worldgenerator.h"
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>

#define MAX_LIGHTS 8

// Set up global data, camera data, and lights
void initializeRenderData(RenderData &renderData) {
  renderData = RenderData{};

  // Global Data
  renderData.globalData = SceneGlobalData{
      .ka = 0.5f,
      .kd = 0.5f,
      .ks = 0.5f,
      .kt = 0.0f // unused
  };

  // Camera Data
  glm::vec3 pos = glm::vec3(50.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  float heightAngle = 30.0f;
  renderData.cameraData = SceneCameraData{
      .pos = glm::vec4(pos, 1.0f),
      .look = -glm::vec4(pos, 0.0f),
      .up = glm::vec4(up, 0.0f),
      .heightAngle = glm::radians(heightAngle),
  };

  // Lights
  SceneLightData directionalLight = SceneLightData{
      .type = LightType::LIGHT_DIRECTIONAL,
      .color = glm::vec4(1.0f),
      .dir = glm::vec4(-3.0f, -2.0f, -1.0f, 0.0f),
  };
  renderData.lights.push_back(directionalLight);
}

Realtime::Realtime(QWidget *parent) : QOpenGLWidget(parent) {
  m_prev_mouse_pos = glm::vec2(size().width() / 2, size().height() / 2);
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);

  m_keyMap[Qt::Key_W] = false;
  m_keyMap[Qt::Key_A] = false;
  m_keyMap[Qt::Key_S] = false;
  m_keyMap[Qt::Key_D] = false;
  m_keyMap[Qt::Key_Control] = false;
  m_keyMap[Qt::Key_Space] = false;

  m_renderData = RenderData();

  m_camera = Camera();

  initializeRenderData(m_renderData);
  m_camera.setData(m_renderData.cameraData);
}

void Realtime::finish() {
  killTimer(m_timer);
  this->makeCurrent();

  glDeleteProgram(m_defaultShader);

  this->doneCurrent();
}

void loadEnemyTextures(std::vector<GLuint>& output) {
    std::array<std::string, 13> imageNames{
        ":/resources/staffImages/anastasio-p.png",
        ":/resources/staffImages/arin-p.png",
        ":/resources/staffImages/daniel-p.png",
        ":/resources/staffImages/evan-p.png",
        ":/resources/staffImages/faisal-p.png",
        ":/resources/staffImages/gavin-p.png",
        ":/resources/staffImages/jean-p.png",
        ":/resources/staffImages/krishi-p.png",
        ":/resources/staffImages/luke-p.png",
        ":/resources/staffImages/praccho-p.png",
        ":/resources/staffImages/sebastian-p.png",
        ":/resources/staffImages/sophie-p.png",
        ":/resources/staffImages/stewart-p.png"
    };
    for (auto fileName : imageNames) {
        QString filepath = QString(fileName.data());

        // Task 1: Obtain image from filepath
        QImage img = QImage(filepath);

        // Task 2: Format image to fit OpenGL
        img = img.convertToFormat(QImage::Format_RGBA8888).mirrored();
        // Task 3: Generate kitten texture
        GLuint newTexture;
        glGenTextures(1, &newTexture);

        // Task 9: Set the active texture slot to texture slot 0
        glActiveTexture(0);
        // Task 4: Bind kitten texture
        glBindTexture(GL_TEXTURE_2D, newTexture);

        // Task 5: Load image into kitten texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

        // Task 6: Set min and mag filters' interpolation mode to linear
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Task 7: Unbind kitten texture
        glBindTexture(GL_TEXTURE_2D, 0);
        output.push_back(newTexture);
    }
}

void Realtime::initializeGL() {
  m_devicePixelRatio = this->devicePixelRatio();

  m_timer = startTimer(1000 / 60);
  m_elapsedTimer.start();

  // Initializing GL.
  // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Error while initializing GL: " << glewGetErrorString(err)
              << std::endl;
  }
  std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION)
            << std::endl;

  // Allows OpenGL to draw objects appropriately on top of one another
  glEnable(GL_DEPTH_TEST);
  // Tells OpenGL to only draw the front face
  glEnable(GL_CULL_FACE);
  // Tells OpenGL how big the screen is
  glViewport(0, 0, size().width() * m_devicePixelRatio,
             size().height() * m_devicePixelRatio);

  m_defaultShader = ShaderLoader::createShaderProgram(
      ":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
  m_cone.initialize();
  m_cube.initialize();
  m_cylinder.initialize();
  m_sphere.initialize();

  m_worldGenerator = WorldGenerator();
  m_worldGenerator.initialize();
  m_worldGenerator.generate();

  // m_postProcessor = PostProcessor();
  // m_postProcessor.initialize(size().width() * m_devicePixelRatio,
  //                            size().height() * m_devicePixelRatio);

  m_skybox = Skybox();
  m_skybox.initialize();

  m_enemyShader = ShaderLoader::createShaderProgram(":/resources/shaders/enemy.vert", ":/resources/shaders/enemy.frag");
  glUseProgram(m_enemyShader);
  std::vector<GLuint> enemyTextures;
  loadEnemyTextures(enemyTextures);
  m_enemyManager = EnemyManager(m_enemyShader, enemyTextures);
  glUseProgram(0);
  bindVbo();
}

void Realtime::loadConstants() {
  GLint kaLoc = glGetUniformLocation(m_defaultShader, "globalData.ka");
  GLint kdLoc = glGetUniformLocation(m_defaultShader, "globalData.kd");
  GLint ksLoc = glGetUniformLocation(m_defaultShader, "globalData.ks");
  glUniform1f(kaLoc, m_renderData.globalData.ka);
  glUniform1f(kdLoc, m_renderData.globalData.kd);
  glUniform1f(ksLoc, m_renderData.globalData.ks);
}

void Realtime::loadCamera() {
  GLint worldSpaceCameraLoc =
      glGetUniformLocation(m_defaultShader, "worldSpaceCamera");
  glm::vec4 worldSpaceCamera =
      glm::inverse(m_camera.getView()) * glm::vec4(0.0, 0.0, 0.0, 1.0);
  glUniform4fv(worldSpaceCameraLoc, 1, &worldSpaceCamera[0]);

  GLint viewMatrixLoc = glGetUniformLocation(m_defaultShader, "viewMatrix");
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.getView()[0][0]);

  GLint projectionMatrixLoc =
      glGetUniformLocation(m_defaultShader, "projectionMatrix");
  glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE,
                     &m_camera.getProjection()[0][0]);
}

void Realtime::loadLights() {
  GLint lightCountLoc = glGetUniformLocation(m_defaultShader, "lightCount");
  int lightCount =
      std::min(static_cast<size_t>(MAX_LIGHTS), m_renderData.lights.size());
  glUniform1i(lightCountLoc, lightCount);

  for (int i = 0; i < lightCount; i++) {
    SceneLightData light = m_renderData.lights[i];

    GLint lightTypeLoc = glGetUniformLocation(
        m_defaultShader, ("lights[" + std::to_string(i) + "].type").c_str());
    GLint lightColorLoc = glGetUniformLocation(
        m_defaultShader, ("lights[" + std::to_string(i) + "].color").c_str());
    GLint lightDirLoc = glGetUniformLocation(
        m_defaultShader, ("lights[" + std::to_string(i) + "].dir").c_str());
    GLint lightPosLoc = glGetUniformLocation(
        m_defaultShader, ("lights[" + std::to_string(i) + "].pos").c_str());
    GLint lightFunctionLoc = glGetUniformLocation(
        m_defaultShader,
        ("lights[" + std::to_string(i) + "].function").c_str());
    GLint lightAngleLoc = glGetUniformLocation(
        m_defaultShader, ("lights[" + std::to_string(i) + "].angle").c_str());
    GLint lightPenumbraLoc = glGetUniformLocation(
        m_defaultShader,
        ("lights[" + std::to_string(i) + "].penumbra").c_str());

    glUniform1i(lightTypeLoc, static_cast<int>(light.type));
    glUniform4fv(lightColorLoc, 1, &light.color[0]);
    glUniform4fv(lightDirLoc, 1, &light.dir[0]);
    glUniform4fv(lightPosLoc, 1, &light.pos[0]);
    glUniform3fv(lightFunctionLoc, 1, &light.function[0]);
    glUniform1f(lightAngleLoc, light.angle);
    glUniform1f(lightPenumbraLoc, light.penumbra);
  }
}

void Realtime::loadShapeData(const RenderShapeData &shapeData) {
  GLint modelMatrixLoc = glGetUniformLocation(m_defaultShader, "modelMatrix");
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &shapeData.ctm[0][0]);

  GLint normalMatrixLoc = glGetUniformLocation(m_defaultShader, "normalMatrix");
  // P * V * M^{-T}
  glm::mat3 normalMatrix =
      glm::inverse(glm::transpose(glm::mat3(shapeData.ctm)));
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  GLint materialAmbientLoc =
      glGetUniformLocation(m_defaultShader, "material.cAmbient");
  GLint materialDiffuseLoc =
      glGetUniformLocation(m_defaultShader, "material.cDiffuse");
  GLint materialSpecularLoc =
      glGetUniformLocation(m_defaultShader, "material.cSpecular");
  GLint materialShininessLoc =
      glGetUniformLocation(m_defaultShader, "material.shininess");
  GLint materialReflectiveLoc =
      glGetUniformLocation(m_defaultShader, "material.cReflective");
  glUniform4fv(materialAmbientLoc, 1,
               &shapeData.primitive.material.cAmbient[0]);
  glUniform4fv(materialDiffuseLoc, 1,
               &shapeData.primitive.material.cDiffuse[0]);
  glUniform4fv(materialSpecularLoc, 1,
               &shapeData.primitive.material.cSpecular[0]);
  glUniform1f(materialShininessLoc, shapeData.primitive.material.shininess);
  glUniform4fv(materialReflectiveLoc, 1,
               &shapeData.primitive.material.cReflective[0]);
}

void Realtime::paintGL() {
  // anything requiring OpenGL calls every frame should be done here
  if (m_settingsChanged) {
    bindVbo();
    m_settingsChanged = false;
  }
  /*m_postProcessor.bindFramebuffer();*/
  /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

  m_skybox.render(m_camera);

  glUseProgram(m_defaultShader);

  loadConstants();
  loadCamera();
  loadLights();

  for (const RenderShapeData &shapeData : m_renderData.shapes) {
    Shape *shape;
    switch (shapeData.primitive.type) {
    case PrimitiveType::PRIMITIVE_CUBE:
      shape = &m_cube;
      break;
    case PrimitiveType::PRIMITIVE_CONE:
      shape = &m_cone;
      break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
      shape = &m_cylinder;
      break;
    case PrimitiveType::PRIMITIVE_SPHERE:
      shape = &m_sphere;
      break;
    default:
      continue;
    }
    shape->bindVao();
    loadShapeData(shapeData);
    glDrawArrays(GL_TRIANGLES, 0, shape->getVertexCount());
    shape->unbindVao();
  }

  m_worldGenerator.bindVao();
  loadShapeData(m_worldGenerator.getShapeData());
  glDrawArrays(GL_TRIANGLES, 0, m_worldGenerator.getVertexCount());
  m_worldGenerator.unbindVao();

  glUseProgram(0);

  m_enemyManager.drawAllEnemy(m_camera.getView(), m_camera.getProjection(), m_camera.getPos());

  /*m_postProcessor.unbindFramebuffer();*/
  /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
  /*m_postProcessor.render(settings.perPixelFilter,
   * settings.kernelBasedFilter);*/

  glErrorCheck();
}

void Realtime::resizeGL(int w, int h) {
  // Tells OpenGL how big the screen is
  glViewport(0, 0, size().width() * m_devicePixelRatio,
             size().height() * m_devicePixelRatio);

  m_camera.setAspectRatio(static_cast<float>(w) / h);
  m_postProcessor.resize(size().width() * m_devicePixelRatio,
                         size().height() * m_devicePixelRatio);
}

void Realtime::settingsChanged() {
  m_settingsChanged = true;

  m_camera.setNearPlane(settings.nearPlane);
  m_camera.setFarPlane(settings.farPlane);

  m_cone.updateParams(settings.shapeParameter1, settings.shapeParameter2);
  m_cube.updateParams(settings.shapeParameter1, settings.shapeParameter2);
  m_cylinder.updateParams(settings.shapeParameter1, settings.shapeParameter2);
  m_sphere.updateParams(settings.shapeParameter1, settings.shapeParameter2);

  update();
}

void Realtime::bindVbo() {
  m_cone.installVbo();
  m_cube.installVbo();
  m_cylinder.installVbo();
  m_sphere.installVbo();

  m_worldGenerator.installVbo();
}

void Realtime::keyPressEvent(QKeyEvent *event) {
  m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
  m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
  if (event->buttons().testFlag(Qt::LeftButton)) {
    m_mouseDown = true;
    m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
  }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
  if (!event->buttons().testFlag(Qt::LeftButton)) {
    m_mouseDown = false;
  }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
  int posX = event->position().x();
  int posY = event->position().y();
  int deltaX = posX - m_prev_mouse_pos.x;
  int deltaY = posY - m_prev_mouse_pos.y;
  m_prev_mouse_pos = glm::vec2(posX, posY);

  const float speed = 0.005f;

  m_camera.rotate(-deltaX * speed, glm::vec3(0, 1, 0));
  m_camera.rotate(-deltaY * speed,
                  glm::cross(m_camera.getLook(), m_camera.getUp()));

  update();
}

void Realtime::timerEvent(QTimerEvent *event) {
  int elapsedms = m_elapsedTimer.elapsed();
  float deltaTime = elapsedms * 0.001f;
  m_elapsedTimer.restart();

  const float speed = 30.0f;

  float factor = speed * deltaTime;
  m_enemyManager.update(deltaTime);
  glm::vec3 look = glm::normalize(m_camera.getLook());
  glm::vec3 up = glm::normalize(m_camera.getUp());

  glm::vec3 moveDir = glm::vec3(0);

  // Use deltaTime and m_keyMap here to move around
  if (m_keyMap[Qt::Key_W]) {
    moveDir += look;
  }
  if (m_keyMap[Qt::Key_A]) {
    moveDir -= glm::normalize(glm::cross(look, up));
  }
  if (m_keyMap[Qt::Key_S]) {
    moveDir -= look;
  }
  if (m_keyMap[Qt::Key_D]) {
    moveDir += glm::normalize(glm::cross(look, up));
  }

  if (glm::length(moveDir) > 0.0f)
    m_camera.movePosition(factor * glm::normalize(moveDir));

  if (m_keyMap[Qt::Key_Control] || m_keyMap[Qt::Key_Shift]) {
    m_camera.movePosition(glm::vec3(0, -factor, 0));
  }
  if (m_keyMap[Qt::Key_Space]) {
    m_camera.movePosition(glm::vec3(0, factor, 0));
  }

  update();
}
