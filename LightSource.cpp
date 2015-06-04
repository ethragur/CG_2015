#include "LightSource.h"
#include "glm/gtx/rotate_vector.hpp"
#include "GraphicsObject.h"


LightSource::LightSource(const glm::vec3 & pos, const glm::vec3 & col, float inten, bool move) :
position(pos),
color(col),
intensity(inten),
moving(move)
{
  
}

void LightSource::move()
{
  float newtime = glutGet(GLUT_ELAPSED_TIME); 
  float angle = (newtime/ (1500* (1/GraphicsObject::speed))) * (360.0/M_PI); 
  position = glm::rotateX(position, (GLfloat)(angle*M_PI/180) / 1000);
  
}
void LightSource::changeColors()
{
    
}
  