#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
  
  
  
  
  //func
  Camera();
  
};

#endif // CAMERA_H
