#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "Matrix.h"
#include <math.h>

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
  float Pos[3];
  std::vector<GLfloat> vertex_buffer_data;
  std::vector<GLfloat> color_buffer_data;
  std::vector<GLushort> index_buffer_data;

    
  //func
  GraphicsObject(std::vector<GLfloat> vertex_buffer_temp, std::vector<GLfloat> color_buffer_temp, std::vector<GLushort> index_buffer_temp);
  void Draw(GLuint ShaderProgram, float ProjectionMatrix[16], float ViewMatrix[16]);
  void SetupDataBuffers();
  void initobj(float x, float y, float z);
  void IdleWork(bool updown);
  
  enum DataID {vPosition = 0, vColor = 1}; 

};
#endif /*GRAPHICSOBJECT_H */
