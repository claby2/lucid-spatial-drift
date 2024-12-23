#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "utils/skybox.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include <unordered_map>

#include "shapes/cone.h"
#include "shapes/cube.h"
#include "shapes/cylinder.h"
#include "shapes/sphere.h"

#include "utils/camera.h"
#include "utils/objectmanager.h"
#include "utils/postprocessor.h"
#include "utils/worldgenerator.h"
#include "utils/collisiondetector.h"

#include "enemies/enemymanager.h"
#include "enemies/projectile.h"

class Realtime : public QOpenGLWidget {
public:
  Realtime(QWidget *parent = nullptr);
  void finish(); // Called on program exit
  void settingsChanged();

public slots:
  void tick(QTimerEvent *event); // Called once per tick of m_timer

protected:
  void initializeGL() override; // Called once at the start of the program
  void paintGL() override; // Called whenever the OpenGL context changes or by
                           // an update() request
  void resizeGL(int width,
                int height) override; // Called when window size changes

private:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void timerEvent(QTimerEvent *event) override;

  void bindVbo();

  // Tick Related Variables
  int m_timer; // Stores timer which attempts to run ~60 times per second
  QElapsedTimer m_elapsedTimer; // Stores timer which keeps track of actual time
                                // between frames

  // Input Related Variables
  bool m_mouseDown = false;   // Stores state of left mouse button
  glm::vec2 m_prev_mouse_pos; // Stores mouse position
  std::unordered_map<Qt::Key, bool>
      m_keyMap; // Stores whether keys are pressed or not

  // Device Correction Variables
  double m_devicePixelRatio;

  RenderData m_renderData;
  Camera m_camera;

  Cone m_cone;
  Cube m_cube;
  Cylinder m_cylinder;
  Sphere m_sphere;

  WorldGenerator m_worldGenerator;

  Skybox m_skybox;
  EnemyManager m_enemyManager;
  ProjectileManager m_projectileManager;

  GLuint m_defaultShader;

  PostProcessor m_postProcessor;

  glm::vec3 GRAVITY_COEFF = glm::vec3(0.f, -0.1f, 0.f);

  void loadConstants();
  void loadCamera();
  void loadLights();
  void loadShapeData(const RenderShapeData &shapeData);

  bool m_settingsChanged = false;

  float m_totalTime = 0.0f;

  bool m_ignoreNextMouseMove = false;
};
