#include "GraphicsObject.h"
#include "glm/ext.hpp"
#include <cmath>
#include <iostream>

float GraphicsObject::speed = 1.0;

GraphicsObject::GraphicsObject(const std::vector<GLfloat> & vertex_buffer_temp, const std::vector<GLfloat> & color_buffer_temp, const std::vector<GLuint> & index_buffer_temp, const std::string &name) :
vertex_buffer_data(vertex_buffer_temp), color_buffer_data(color_buffer_temp), index_buffer_data(index_buffer_temp), name(name) 
{ 
  
}


/********************************************
 * Initialize objects
 * starting Pos, Rotation....
 * 
*****++++++++++++++++++++++++++++++++++++++++*/
void GraphicsObject::initobj(float x, float y, float z)
{
  SetupDataBuffers();
  ModelMatrix = glm::mat4(1.0);
  TranslateOrigin = glm::translate(glm::vec3(x,y,z));
  ModelMatrix = TranslateOrigin * ModelMatrix;

}


void GraphicsObject::IdleWork(bool updown)
{
  //compute FPS independent TIME
  oldtime = newtime;
  newtime = glutGet(GLUT_ELAPSED_TIME); 
  if(!(name.compare("back1") == 0 || name.compare("Plane") == 0))
  {
    rotAroundCenter();
  
    if(updown)
    {
      UpDown();
    }
  }
}



/* Updown Movement Function for the Horses
 * 
 * */
void GraphicsObject::UpDown()
{
  TranslateOrigin = glm::translate(glm::vec3(0,sin((newtime/10)*(M_PI/180)),0));
  ModelMatrix = TranslateOrigin * ModelMatrix;
}


/* Rotation Function for all GraphicsObjects
 * 
 * */
void GraphicsObject::rotAroundCenter()
{
  float angle = (newtime/ (1500* (1/GraphicsObject::speed))) * (360.0/M_PI); 
  
  glm::mat4 RotationMatrixAnim;
  RotationMatrixAnim = glm::rotate((GLfloat)(angle*M_PI/180)   , glm::vec3(0,1,0));
  
  ModelMatrix = RotationMatrixAnim * InitialTransform;
}


/******************************************************************
*
* SetupDataBuffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/
void GraphicsObject::SetupDataBuffers()
{
  
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, color_buffer_data.size() * sizeof(GLfloat), &color_buffer_data[0], GL_STATIC_DRAW);
   
}


/*
 * Draw Object to screen
 * */
void GraphicsObject::Draw(GLuint ShaderProgram, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(vColor);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);   

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    /* Associate program with shader matrices */
    GLint projectionUniform = glGetUniformLocation(ShaderProgram, "ProjectionMatrix");
    if (projectionUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ProjectionMatrix\n");
	exit(-1);
    }
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    
    GLint ViewUniform = glGetUniformLocation(ShaderProgram, "ViewMatrix");
    if (ViewUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ViewMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
   
    GLint RotationUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
    if (RotationUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ModelMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(RotationUniform, 1, GL_FALSE, glm::value_ptr(ModelMatrix));  

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);

    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);   
}
