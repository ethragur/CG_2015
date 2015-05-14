#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
  float fovy ;
  float aspect ; 
  float nearPlane ; 
  float farPlane ;
  float camera_disp;
  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  int oldtime;
  int newtime;
  float camX;
  float camZ;
  
  //func
  Camera();
  void move(glm::vec3);
  void rotate(glm::vec3);
  void freeFly();
  
};

#endif // CAMERA_H
