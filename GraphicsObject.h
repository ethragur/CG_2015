#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class GraphicsObject
{

public:
  //vars
  GLuint VBO;
  GLuint CBO;
  GLuint IBO;
  float TranslateOrigin[16];
  float TranslateDown[16];
  float TranslateX[16];
  float RotateX[16];
  float RotateZ[16];
  float InitialTransform[16];
  float ModelMatrix[16];  /* Model matrix */ 
  GLfloat vertex_buffer_data[];
  GLfloat color_buffer_data[];
  GLushort index_buffer_data[];

    
  //func
  GraphicsObject(GLfloat vertex_buffer_temp[], GLfloat color_buffer_temp[], GLushort index_buffer_temp[]);
  void Draw(GLuint ShaderProgram, float ProjectionMatrix[16], float ViewMatrix[16]);
  //void SetupDataBuffers();
  
  
  enum DataID {vPosition = 0, vColor = 1}; 

};
#endif /*GRAPHICSOBJECT_H */
