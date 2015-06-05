#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>

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
  initialViewMatrix = ViewMatrix;
  factor = 1;
  
}


//moves the camera to a point
void Camera::move(glm::vec3 transVec)
{
    glm::mat4 translation(1.0f);
    translation = glm::translate(transVec);
    ViewMatrix = translation * ViewMatrix;
}

//rotates the Camera by a vec3
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

//rotates the camera around the screens center
void Camera::rotateScreenMid(glm::vec3 rotation)
{
  glm::mat4 mRotation(1.0f);
  if(rotation.x != 0.0f) 
	  mRotation = glm::rotate(mRotation, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  if(rotation.y != 0.0f)
	  mRotation = glm::rotate(mRotation, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  if(rotation.z != 0.0f)
	  mRotation = glm::rotate(mRotation, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  ViewMatrix *= mRotation;
}


//lookAt the current ScreenMid
void Camera::lookAtCenter()
{
  glm::mat4 oldView = ViewMatrix;
  //ViewMatrix = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0},oldView[3]};
  
}

//camera Moves around in a pattern
void Camera::freeFly()
{
  oldtime = newtime;
  newtime = glutGet(GLUT_ELAPSED_TIME); 
  //
  if(ViewMatrix[1][1] > 0.95 || ViewMatrix[1][1] < 0.85)
    rotateScreenMid(glm::vec3(0.002,0.0, 0));
  else
  {
    rotateScreenMid(glm::vec3(0,-0.002, 0));
    //rotateScreenMid(glm::vec3(sin((newtime/10)*(M_PI/180)) / 20), 0, 0);
  }
 
  move(glm::vec3((sin((newtime/10)*(M_PI/180)) / 20) * factor, (sin((newtime/10)*(M_PI/180)) / 20) * factor ,0.0));
  
  move(glm::vec3(0.0,0.0,(sin((newtime/10)*(M_PI/180)) / 20) * factor ));

  
  
  //lookAtCenter();
 
  
  
  //std::cout << glm::to_string(ViewMatrix) << std::endl;
  
}




