#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Camera
{
public:
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
  glm::mat4 initialViewMatrix;
  float fovy ;
  float aspect ; 
  float nearPlane ; 
  float farPlane ;
  float camera_disp;
  int oldtime;
  int newtime;
  float factor;

  //func
  Camera();
  void move(glm::vec3);
  void rotate(glm::vec3);
  void freeFly();
  void rotateScreenMid(glm::vec3);
  void lookAtCenter();
  
};

#endif // CAMERA_H
