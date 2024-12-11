#pragma once
#include "sceneparser.h"
#include "utils/collisiondetector.h"
#include <iostream>

class Camera {
public:
  Camera() = default;
  Camera(const SceneCameraData &data, float nearPlane, float farPlane);

  glm::mat4 getView() const;
  glm::mat4 getProjection() const;
  glm::vec3 getLook() const;
  glm::vec3 getUp() const;
  const glm::vec3 getPos() { return m_pos; }

  void setData(const SceneCameraData &data);
  void setNearPlane(float nearPlane);
  void setFarPlane(float farPlane);
  void setAspectRatio(float aspectRatio);
  void movePosition(glm::vec3 delta);
  void rotate(float angle, glm::vec3 axis);

  void collideAndSlide(std::vector<float>& vertexData, const glm::vec3 v, const glm::vec3 g);

  void printVec3(glm::vec3 vec) { std::cout << std::to_string(m_pos[0]) + ", " + std::to_string(m_pos[1]) + ", " + std::to_string(m_pos[2]) << std::endl; }

private:
  glm::vec3 m_pos;
  glm::vec3 m_look;
  glm::vec3 m_up;
  float m_heightAngle;

  float m_nearPlane;
  float m_farPlane;
  float m_aspectRatio;

  // Collisions

  CollisionDetector m_collisionDetector;

  glm::vec3 playerSize = glm::vec3(1.f, 2.f, 1.f); // move out of this class

  glm::vec3 collideWithWorld(int depth, std::vector<float>& vertexData, CollisionDetector::CollisionPacket collisionPackage);
};
