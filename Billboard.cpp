#include "Billboard.h"



void Billboard::IdleWork ( bool updown )
{
  glm::vec3 currentPos = glm::vec3(ModelMatrix[3][0], ModelMatrix[3][1], ModelMatrix[3][2]);
  glm::vec3 look = glm::normalize(Camera::getInstance()->cameraPos - currentPos);
  glm::vec3 right = glm::cross(glm::vec3(0,1,0), look);
  glm::vec3 up2 = glm::cross(look, right);
  glm::mat4 transform;
  transform[0] = glm::vec4(right, 0);
  transform[1] = glm::vec4(up2, 0);
  transform[2] = glm::vec4(look, 0);
  ModelMatrix = transform;
}
