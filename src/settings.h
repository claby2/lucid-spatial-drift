#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
  int shapeParameter1 = 5;
  int shapeParameter2 = 5;
  float nearPlane = 0.1f;
  float farPlane = 100.0f;
  bool perPixelFilter = false;
  bool kernelBasedFilter = false;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
