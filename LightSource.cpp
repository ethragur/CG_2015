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
  oldtime = newtime;
  newtime = glutGet(GLUT_ELAPSED_TIME); 
  float angle = ((newtime-oldtime) / (1500* (1/GraphicsObject::speed))) * (360.0/M_PI); 
  
  glm::mat4 RotationMatrixAnim;
  RotationMatrixAnim = glm::rotate((GLfloat)(angle*M_PI/180)   , glm::vec3(0,1,0));
  
  glm::vec4 test = RotationMatrixAnim * glm::vec4(position, 0);
  
  position = glm::vec3(test.x, test.y, test.z);
  
}
void LightSource::changeColors()
{
    
}
  