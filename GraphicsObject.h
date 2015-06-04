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
#include "camera.h"
#include "LightSource.h"

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
  std::vector<GLuint> index_buffer_data;
  std::vector<GLfloat> vertex_normal_data;
  
  glm::vec3 diffuse;
  glm::vec3 specular;
  GLfloat shininess;
  
  static float speed;
  static bool disableSpec;
  static bool disableAmbient;
  static bool disableDiff;

    
  //func
  GraphicsObject(const std::vector<GLfloat> & vertex_buffer_temp, 
		 const std::vector<GLuint> & index_buffer_temp, 
		 const glm::vec3 & diff_tmp, 
		 const glm::vec3 & spec_tmp,
		 GLfloat shiny,
		 const std::string & name);
  void Draw(GLuint ShaderProgram, std::vector<LightSource> lightSources);
  void initobj(float x, float y, float z);
  void IdleWork(bool updown);
  
  enum DataID {vPosition = 0, vNormals = 1}; 
  
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
