#ifndef INITCLASS_H
#define INITCLASS_H
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>



using namespace std;
class InitClass
{
public:
  
  /* Define handle to a vertex buffer object */
    GLuint VBO;

  /* Define handle to a color buffer object */
  GLuint CBO; 

  /* Define handle to an index buffer object */
  GLuint IBO;


  /* Indices to vertex attributes; in this case positon and color */ 
  enum DataID {vPosition = 0, vColor = 1}; 

  /* Strings for loading and storing shader code */
  static const char* VertexShaderString;
  static const char* FragmentShaderString;

  GLuint ShaderProgram;
  static void Initialize();
  static void CreateShaderProgram();
  static void SetupDataBuffers();
};



#endif /*INITCLASS_H*/