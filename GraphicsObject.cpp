#include "GraphicsObject.h"

GraphicsObject::GraphicsObject(std::vector<GLfloat> vertex_buffer_temp, std::vector<GLfloat> color_buffer_temp, std::vector<GLushort> index_buffer_temp)
{	
  
  vertex_buffer_data = vertex_buffer_temp;
  

  color_buffer_data = color_buffer_temp;
  

  index_buffer_data = index_buffer_temp;
  
}


/********************************************
 * Initialize objects
 * starting Pos, Rotation....
 * 
*****++++++++++++++++++++++++++++++++++++++++*/

void GraphicsObject::initobj(float x, float y, float z)
{
  SetupDataBuffers();
  SetIdentityMatrix(ModelMatrix);
  SetTranslation(x, y, z, TranslateOrigin);
  Pos[0] = stPos[0] = x;
  Pos[1] = stPos[1] = y;
  Pos[2] = stPos[2] = z;
  SetRotationX(0, RotateX);
  SetRotationZ(0, RotateZ);	
  MultiplyMatrix(RotateX, TranslateOrigin, InitialTransform);
  MultiplyMatrix(RotateZ, InitialTransform, InitialTransform);
  MultiplyMatrix(TranslateOrigin, ModelMatrix, ModelMatrix);
}


void GraphicsObject::IdleWork(bool updown)
{
  float angle = (glutGet(GLUT_ELAPSED_TIME) / 1500.0) * (360.0/M_PI); 
  float RotationMatrixAnim[16];
  //should object move up and down
  if(updown)
  {
    SetRotationY(angle, RotationMatrixAnim);
    
    if(down)
    {
      Pos[1] += 0.1f;
      if(Pos[1] >= 3 - stPos[1])
      {
	//for float correction
	Pos[1] = 3 - stPos[1];
	down = false;
      }
    }
    else
    {
      Pos[1] -= 0.1f;
      if(Pos[1] <= -3 - stPos[1])
      {
	Pos[1] = -3 - stPos[1];
	down = true;
      }
    }
    SetTranslation(0, Pos[1], 0,TranslateOrigin);
    
    MultiplyMatrix(RotationMatrixAnim, InitialTransform, ModelMatrix);
    MultiplyMatrix(TranslateOrigin, ModelMatrix, ModelMatrix);
   
    
  }
  else
  {
    SetRotationY(-angle, RotationMatrixAnim);
    MultiplyMatrix(RotationMatrixAnim, InitialTransform, ModelMatrix);
  }
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
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLfloat), &vertex_buffer_data[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLushort), &index_buffer_data[0], GL_STATIC_DRAW);

    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLfloat), &color_buffer_data[0], GL_STATIC_DRAW);
   
}



void GraphicsObject::Draw(GLuint ShaderProgram, float ProjectionMatrix[16], float ViewMatrix[16])
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
    glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, ProjectionMatrix);
    
    GLint ViewUniform = glGetUniformLocation(ShaderProgram, "ViewMatrix");
    if (ViewUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ViewMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(ViewUniform, 1, GL_TRUE, ViewMatrix);
   
    GLint RotationUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
    if (RotationUniform == -1) 
    {
        fprintf(stderr, "Could not bind uniform ModelMatrix\n");
        exit(-1);
    }
    glUniformMatrix4fv(RotationUniform, 1, GL_TRUE, ModelMatrix);  

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);   
}