#pragma once
#include "sceneparser.h"

class Camera {
public:
  Camera() = default;
  Camera(const SceneCameraData &data, float nearPlane, float farPlane);

  glm::mat4 getView() const;
  glm::mat4 getProjection() const;
  glm::vec3 getLook() const;
  glm::vec3 getUp() const;

  void setData(const SceneCameraData &data);
  void setNearPlane(float nearPlane);
  void setFarPlane(float farPlane);
  void setAspectRatio(float aspectRatio);
  void movePosition(glm::vec3 delta);
  void rotate(float angle, glm::vec3 axis);

private:
  glm::vec3 m_pos;
  glm::vec3 m_look;
  glm::vec3 m_up;
  float m_heightAngle;

  float m_nearPlane;
  float m_farPlane;
  float m_aspectRatio;
};
