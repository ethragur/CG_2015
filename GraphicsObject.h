#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <math.h>

class GraphicsObject
{

public:
  //vars
  std::string name;
  GLuint VBO;
  GLuint CBO;
  GLuint IBO;
  GLuint NBO;
  glm::mat4 TranslateOrigin;
  glm::mat4 InitialTransform;
  glm::mat4 ModelMatrix;
  std::vector<GLfloat> vertex_buffer_data;
  std::vector<GLfloat> color_buffer_data;
  std::vector<GLuint> index_buffer_data;
  std::vector<GLfloat> vertex_normal_data;
  static float speed;

    
  //func
  GraphicsObject(const std::vector<GLfloat> & vertex_buffer_temp, const std::vector<GLfloat> & color_buffer_temp, const std::vector<GLuint> & index_buffer_temp, const std::vector<GLfloat> & vertex_normal_temp, const std::string & name);
  void Draw(GLuint ShaderProgram, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
  void initobj(float x, float y, float z);
  void IdleWork(bool updown);
  
  enum DataID {vPosition = 0, vColor = 1, vNormals = 2}; 
  
private:
  //vars
  
  float oldtime;
  float newtime;
  
  //func
  void rotAroundCenter();
  void UpDown();
  void SetupDataBuffers();
};
#endif /*GRAPHICSOBJECT_H */
