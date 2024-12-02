#pragma once

#include "shape.h"

class Cylinder : public Shape {
public:
  void makeShape() override;

private:
  void makeBodyTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                    glm::vec3 bottomRight);
  void makeCapTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                   glm::vec3 bottomRight, glm::vec3 normal);
  void makeBody();
  void makeCap(float y, glm::vec3 normal);
};
