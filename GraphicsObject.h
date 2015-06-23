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
  std::string texname;
  std::string normtexname;
  
  //vertex
  GLuint VBO;
  //color
  GLuint CBO;
  //indicies
  GLuint IBO;
  //normals
  GLuint NBO;
  //uvs
  GLuint UBO;
  //tangents
  GLuint TBO;
  //bitangents
  GLuint BBO;
  
  GLint textureID;
  GLint normalMapID;
  
  glm::vec3 position;
  
  glm::mat4 TranslateOrigin;
  glm::mat4 InitialTransform;
  glm::mat4 ModelMatrix;
  
  std::vector<GLfloat> vertex_buffer_data;
  std::vector<GLuint> index_buffer_data;
  std::vector<GLfloat> vertex_normal_data;
  std::vector<GLfloat> texture_vertex_data;
  std::vector<GLfloat> vertex_tangent_data;
  std::vector<GLfloat> vertex_bintangent_data;
  
  
  glm::vec3 diffuse;
  glm::vec3 specular;
  GLfloat shininess;
  
  //Shader Flags
  static float speed;
  static bool disableSpec;
  static bool disableAmbient;
  static bool disableDiff;
  static bool disableNormalMapping;

    
  //func
  GraphicsObject(const std::vector<GLfloat> & vertex_buffer_temp, 
		 const std::vector<GLuint> & index_buffer_temp, 
		 const glm::vec3 & diff_tmp, 
		 const glm::vec3 & spec_tmp,
		 const std::vector<GLfloat> & tex_tmp,
		 GLfloat shiny,
		 const std::string & name,
		 const std::string & texname,
		 const std::string & normtexname);
  
  void Draw(GLuint ShaderProgram, std::vector<LightSource> lightSources);
  void initobj(float x, float y, float z);
  void IdleWork(bool updown);
  
  GLint LoadTexture(const std::string & filename);
  
  enum DataID {vPosition = 0, vNormals = 1, vUV = 2, vTangents = 3, vBiTangents = 4}; 
  
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
