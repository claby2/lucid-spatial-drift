#pragma once

#include "shape.h"

class Cube : public Shape {
public:
  void makeShape() override;

private:
  void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                glm::vec3 bottomRight);
  void makeFace(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft,
                glm::vec3 bottomRight);
};
