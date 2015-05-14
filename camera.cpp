#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

Camera::Camera()
{
 ViewMatrix = glm::mat4(1.0);
 ProjectionMatrix = glm::mat4(1.0);
 float fovy = 45.0;
 float aspect = 1.0; 
 float nearPlane = 1.0; 
 float farPlane = 1000.0;
 ProjectionMatrix = glm::perspective(fovy, aspect, nearPlane, farPlane);
 
 /* Set viewing transform */
  float camera_disp = -20.0;
  ViewMatrix = glm::translate(glm::vec3(0.0,0.0, camera_disp));
}
