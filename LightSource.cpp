#include "LightSource.h"
#include "glm/gtx/rotate_vector.hpp"
#include "GraphicsObject.h"
#include <iostream>

GLfloat LightSource::intensity = 1.0f;

LightSource::LightSource(const glm::vec3 & pos, const glm::vec3 & col, bool move) :
position(pos),
color(col),
moving(move)
{
  
}

void LightSource::move()
{
  float newtime = glutGet(GLUT_ELAPSED_TIME); 
  //float angle = ((newtime/ (1500* (1/GraphicsObject::speed))) * (360.0/M_PI)) %360;
  float x = position.x;
  float z = position.z;
  
  position.z = z*cos(newtime) + x * sin(newtime);
  position.x = z*sin(newtime) + x * cos(newtime);
  
  std::cout << x << " " << z << std::endl;
  
}
void LightSource::changeColors()
{
    
}
  