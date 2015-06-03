#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "GraphicsObject.h"
#include <vector>
#include <iostream>

class LightSource : GraphicsObject
{
public:
  glm::vec3 position;
  glm::vec3 color;
};


#endif //LIGHTSOURCE_H