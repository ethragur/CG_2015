#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include <GL/gl.h>

class Camera
{

private:
  static Camera *inst;
public:
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
  glm::mat4 initialViewMatrix;
  glm::vec3 cameraPos;
  float fovy ;
  float aspect ; 
  float nearPlane ; 
  float farPlane ;
  float camera_disp;
  int oldtime;
  int newtime;
  float factor;
  
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  GLfloat Yaw;
  GLfloat Pitch;
  
  
  //func
  Camera();
  static Camera* getInstance();
  void updateCameraVectors();
  glm::mat4 GetViewMatrix();
  void move(char dir);
  void rotate(GLfloat, GLfloat);
  
};

#endif // CAMERA_H
