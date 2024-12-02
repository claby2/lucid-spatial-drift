#pragma once

#include "shape.h"

class Cone : public Shape {
public:
  void makeShape() override;

private:
  void makeCap();
  void makeBody();
  void makeCapTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                   glm::vec3 bottomRight, glm::vec3 normal);
  void makeBodyTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, bool is_top);
};
