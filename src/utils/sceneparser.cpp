#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

// Traverse the scene graph and collect all the shapes and lights
void traverse(SceneNode *node, glm::mat4 ctm,
              std::vector<RenderShapeData> &shapes,
              std::vector<SceneLightData> &lights) {
  for (SceneTransformation *t : node->transformations) {
    switch (t->type) {
    case TransformationType::TRANSFORMATION_TRANSLATE:
      ctm = glm::translate(ctm, t->translate);
      break;
    case TransformationType::TRANSFORMATION_ROTATE:
      ctm = glm::rotate(ctm, t->angle, t->rotate);
      break;
    case TransformationType::TRANSFORMATION_SCALE:
      ctm = glm::scale(ctm, t->scale);
      break;
    case TransformationType::TRANSFORMATION_MATRIX:
      ctm = t->matrix * ctm;
      break;
    default:
      break;
    }
  }

  for (ScenePrimitive *primitive : node->primitives) {
    shapes.push_back(RenderShapeData{*primitive, ctm});
  }

  for (SceneLight *light : node->lights) {
    lights.push_back(SceneLightData{
        light->id, light->type, light->color, light->function,
        ctm * glm::vec4(0.0, 0.0, 0.0, 1.0), ctm * light->dir, light->penumbra,
        light->angle, light->width, light->height});
  }

  for (SceneNode *children : node->children) {
    traverse(children, ctm, shapes, lights);
  }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
  ScenefileReader fileReader = ScenefileReader(filepath);
  bool success = fileReader.readJSON();
  if (!success) {
    return false;
  }

  renderData.globalData = fileReader.getGlobalData();
  renderData.cameraData = fileReader.getCameraData();

  SceneNode *root = fileReader.getRootNode();
  renderData.shapes.clear();

  glm::mat4 ctm = glm::mat4(1.0f);
  traverse(root, ctm, renderData.shapes, renderData.lights);

  return true;
}
