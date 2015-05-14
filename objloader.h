#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "GraphicsObject.h"
#include <vector>
#include <iostream>

class ObjLoader
{
public:
  static std::vector<GraphicsObject> loadObj( const std::string& objpath, const std::string& mtlpath );
};


#endif //OBJLOADER_H