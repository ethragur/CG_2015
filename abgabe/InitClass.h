#ifndef INITCLASS_H
#define INITCLASS_H

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>




class InitClass
{
public:
  static int USES_MESA;
  /*----------------------------------------------------------------*/
  /* Define handle to a vertex array object (only for MESA USE) */
  static GLuint VAO;
  /* Strings for loading and storing shader code */
  static const char* VertexShaderString;
  static const char* FragmentShaderString;
  static GLuint ShaderProgram;
  
  
  //func
  static void setupIntelMesaConfiguration();
  static const char* LoadShader(const char*);
  static void CreateShaderProgram();
  static void AddShader(GLuint ShaderProgram, const char* ShaderCode, GLenum ShaderType);
  //for MESA
  static void setupArrayObject();
};


#endif /*INITCLASS_H*/