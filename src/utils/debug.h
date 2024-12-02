#pragma once
#include <GL/glew.h>
#include <iostream>

namespace Debug {
inline void glErrorCheck(const char *fileName, int lineNumber) {
  GLenum errorNumber = glGetError();
  while (errorNumber != GL_NO_ERROR) {
    std::cout << fileName << "(" << lineNumber << "): " << errorNumber
              << std::endl;

    errorNumber = glGetError();
  }
}
} // namespace Debug

#define glErrorCheck() Debug::glErrorCheck(__FILE__, __LINE__)
