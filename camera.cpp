#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>

Camera *Camera::inst = NULL;

Camera::Camera() : Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
  fovy = 45.0;
  aspect = 1.0; 
  nearPlane = 1.0; 
  farPlane = 1000.0;
  ProjectionMatrix = glm::perspective(fovy, aspect, nearPlane, farPlane);
  
  this->Position = glm::vec3(0.0f, 2.0f, 6.0f);
  this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->Yaw = -90.0f;
  this->Pitch = 0.0f;
  this->updateCameraVectors();
}


// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}


void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}

Camera* Camera::getInstance()
{
  if(inst == NULL)
  {
    inst = new Camera();
  }
  return inst;
}

void Camera::move(char dir)
{
  if(dir == 'w')
  {
      this->Position += this->Front * 0.1;
  }
  else if(dir == 's')
  {
      this->Position -= this->Front * 0.1;
  }
  else if(dir == 'a')
  {
    this->Position -= this->Right * 0.1;
  }
  else
  {
    this->Position += this->Right * 0.1;
  }
      
}

void Camera::rotate(GLfloat x, GLfloat y)
{
  this->Yaw   += x;
  this->Pitch -= y;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (true)
  {
      if (this->Pitch > 89.0f)
	  this->Pitch = 89.0f;
      if (this->Pitch < -89.0f)
	  this->Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  this->updateCameraVectors();
}




