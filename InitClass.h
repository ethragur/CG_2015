#ifndef INITCLASS_H
#define INITCLASS_H
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>




class InitClass
{
public:
  


  /* Strings for loading and storing shader code */
  static const char* VertexShaderString;
  static const char* FragmentShaderString;

  GLuint ShaderProgram;
  static void LoadShader();
  static void CreateShaderProgram();

};



#endif /*INITCLASS_H*/