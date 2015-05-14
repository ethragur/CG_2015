#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <cmath>
#include <GL/freeglut.h>
Camera::Camera()
{
 ViewMatrix = glm::mat4(1.0);
 ProjectionMatrix = glm::mat4(1.0);
 fovy = 45.0;
 aspect = 1.0; 
 nearPlane = 1.0; 
 farPlane = 1000.0;
 ProjectionMatrix = glm::perspective(fovy, aspect, nearPlane, farPlane);
 
  /* Set viewing transform */
  camera_disp = -15.0;
  ViewMatrix = glm::translate(glm::vec3(0.0,0.0, camera_disp));
  move(glm::vec3(0,-2,0));
  
}



void Camera::move(glm::vec3 transVec)
{
    glm::mat4 translation(1.0f);
    translation = glm::translate(transVec);
    ViewMatrix = translation * ViewMatrix;
}


void Camera::rotate(glm::vec3 rotation)
{
  glm::mat4 mRotation(1.0f);
  if(rotation.x != 0.0f) 
	  mRotation = glm::rotate(mRotation, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  if(rotation.y != 0.0f)
	  mRotation = glm::rotate(mRotation, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  if(rotation.z != 0.0f)
	  mRotation = glm::rotate(mRotation, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  ViewMatrix = mRotation * ViewMatrix;
}


void Camera::freeFly()
{
  ViewMatrix = glm::translate(glm::vec3(0,0,0));
  move(glm::vec3(3.5,0,0));
  float angle = (glutGet(GLUT_ELAPSED_TIME) / 1500.0) * (360.0/M_PI); 
  glm::mat4 RotationMatrixAnim;
  RotationMatrixAnim = glm::rotate((GLfloat)(angle*M_PI/180), glm::vec3(0,1,0));
  ViewMatrix = RotationMatrixAnim * ViewMatrix;
}




