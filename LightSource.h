#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class LightSource
{
public:
  glm::vec3 position;
  glm::vec3 color;
  static GLfloat intensity;
  
  float newtime;
  float oldtime;
  
  bool moving;
  
  LightSource(const glm::vec3 & pos, const glm::vec3 & col, bool move);
  void move();
  void changeColors();
  
  
};


#endif //LIGHTSOURCE_H