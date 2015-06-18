#include "GraphicsObject.h"
#include "glm/ext.hpp"
#include <cmath>
#include <iostream>


float GraphicsObject::speed = 1.0;
bool GraphicsObject::disableSpec = false;
bool GraphicsObject::disableDiff = false;
bool GraphicsObject::disableAmbient = false;

GraphicsObject::GraphicsObject(
  const std::vector<GLfloat> & vertex_buffer_temp, 
  const std::vector<GLuint> & index_buffer_temp,
  const glm::vec3 & diff_tmp, 
  const glm::vec3 & spec_tmp,
  const std::vector<GLfloat> & tex_tmp,
  GLfloat shiny,
  const std::string &name,
  const std::string &texname) :
vertex_buffer_data(vertex_buffer_temp),
index_buffer_data(index_buffer_temp),
diffuse(diff_tmp),
specular(spec_tmp),
texture_vertex_data(tex_tmp),
shininess(shiny),
name(name),
texname(texname)
{ 
  //calculations of vertex normals
  std::vector<glm::vec3> norm(vertex_buffer_temp.size() / 3, glm::vec3(0,0,0));
  
  for(int i = 0; i < index_buffer_temp.size() ; i+=3)
  {
    glm::vec3 v1 = glm::vec3(vertex_buffer_temp[index_buffer_temp[i+0]*3+0], vertex_buffer_temp[index_buffer_temp[i+0]*3+1],vertex_buffer_temp[index_buffer_temp[i+0]*3+2]);
    glm::vec3 v2 = glm::vec3(vertex_buffer_temp[index_buffer_temp[i+1]*3+0], vertex_buffer_temp[index_buffer_temp[i+1]*3+1],vertex_buffer_temp[index_buffer_temp[i+1]*3+2]);
    glm::vec3 v3 = glm::vec3(vertex_buffer_temp[index_buffer_temp[i+2]*3+0], vertex_buffer_temp[index_buffer_temp[i+2]*3+1],vertex_buffer_temp[index_buffer_temp[i+2]*3+2]);
    glm::vec3 fn = glm::normalize(glm::cross(v3-v2,v1-v2));
    norm[(index_buffer_temp[i])+0] += fn; 
    norm[(index_buffer_temp[i])+1] += fn;
    norm[(index_buffer_temp[i])+2] += fn;
  }
  
  std::vector<GLfloat> resNorm(vertex_buffer_temp.size(), 0);
  
  for(int i = 0; i < norm.size(); i++)
  {
    resNorm[i*3+0] = norm[i].x;
    resNorm[i*3+1] = norm[i].y;
    resNorm[i*3+2] = norm[i].z;
  }
  
  vertex_normal_data = resNorm;
  
  
  
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
  
  //load texture from materials
  std::string texpath = "../res/" + texname;
  textureID = LoadTexture("../res/fur4.bmp");
}


void GraphicsObject::IdleWork(bool updown)
{
  //compute FPS independent TIME
  oldtime = newtime;
  newtime = glutGet(GLUT_ELAPSED_TIME) ; 
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
  TranslateOrigin = glm::translate(glm::vec3(0,sin(((newtime)/10)*(M_PI/180)),0));
  ModelMatrix = TranslateOrigin * ModelMatrix;
}


/* Rotation Function for all GraphicsObjects
 * 
 * */
void GraphicsObject::rotAroundCenter()
{
  float angle = ((newtime) / (1500* (1/GraphicsObject::speed))) * (360.0/M_PI); 
  
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

    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_normal_data.size() * sizeof(GLfloat), &vertex_normal_data[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, texture_vertex_data.size() * sizeof(GLfloat), &texture_vertex_data[0], GL_STATIC_DRAW);
 
}


/*
 * Draw Object to screen
 * intialize UNIFORMS
 * and ATTRIBUTES
 * */
void GraphicsObject::Draw(GLuint ShaderProgram, std::vector<LightSource> lightSources)
{
    
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glEnableVertexAttribArray(vNormals);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glVertexAttribPointer(vNormals,3,GL_FLOAT,GL_FALSE,0,0);
    
    glEnableVertexAttribArray(vUV);
    glBindBuffer(GL_ARRAY_BUFFER, UBO);
    glVertexAttribPointer(vUV,2, GL_FLOAT, GL_FALSE,0,0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);  

    //create Lightning vectors for FragmentShader
    std::vector<GLfloat> lightPositionMat(lightSources.size() * 3, 0);
    std::vector<GLfloat> lightColorMat(lightSources.size() * 3, 0);
    std::vector<GLfloat> lightIntensity(lightSources.size(), 0);
    
    for(int i = 0; i < lightSources.size() * 3; i += 3)
    {    
	
	lightPositionMat[i] = lightSources.at(i/3).position.x;
	lightPositionMat[i+1] = lightSources.at(i/3).position.y;
	lightPositionMat[i+2] = lightSources.at(i/3).position.z;
	
	lightColorMat[i] = lightSources.at(i/3).color.r;
	lightColorMat[i+1] = lightSources.at(i/3).color.g;
	lightColorMat[i+2] = lightSources.at(i/3).color.b;
	
	lightIntensity[i/3] = lightSources.at(i/3).intensity;
    }
    
    GLint lightColorLoc  = glGetUniformLocation(ShaderProgram, "lightColor");
    glUniform3fv(lightColorLoc,  lightSources.size(), &lightColorMat[0]); 
    
    GLint lightPosLoc = glGetUniformLocation(ShaderProgram, "lightPos");
    glUniform3fv(lightPosLoc, lightSources.size(), &lightPositionMat[0]);
    
    GLint lightIntensitLoc = glGetUniformLocation(ShaderProgram, "lightIntensity");
    glUniform1fv(lightIntensitLoc, lightSources.size(),  &lightIntensity[0]);
  
    
    GLint cameraPosShader = glGetUniformLocation(ShaderProgram, "camerPos"); 
    glUniform3f(cameraPosShader, Camera::getInstance()->cameraPos.x, Camera::getInstance()->cameraPos.y, Camera::getInstance()->cameraPos.z);
 

    GLint diffColor = glGetUniformLocation(ShaderProgram, "objectColor");
    glUniform3f(diffColor, diffuse.r, diffuse.g, diffuse.b);
    
    GLint shiny = glGetUniformLocation(ShaderProgram, "shininess");
    glUniform1f(shiny, shininess);
    
    GLint specColor = glGetUniformLocation(ShaderProgram, "specColor");
    glUniform3f(specColor, specular.r, specular.g, specular.b);
  
    
    GLint specLightning = glGetUniformLocation(ShaderProgram, "disableSpec");
    glUniform1i(specLightning, GraphicsObject::disableSpec);
    
    GLint diffLightning = glGetUniformLocation(ShaderProgram, "disableDiff");
    glUniform1i(diffLightning, GraphicsObject::disableDiff);
    
    GLint ambiLightning = glGetUniformLocation(ShaderProgram, "disableAmbi");
    glUniform1i(ambiLightning,  GraphicsObject::disableAmbient);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLint texSampler = glGetUniformLocation(ShaderProgram, "texSampler");
    glUniform1i(texSampler, 0);
    
    
    
    
    GLint projectionUniform = glGetUniformLocation(ShaderProgram, "ProjectionMatrix");
    if (projectionUniform == -1) 
    {
	std::cerr << "Could not bind uniform ProjectionMatrix " << std::endl;
	exit(-1);
    }
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(Camera::getInstance()->ProjectionMatrix));
    
    GLint ViewUniform = glGetUniformLocation(ShaderProgram, "ViewMatrix");
    if (ViewUniform == -1) 
    {
        std::cerr << "Could not bind uniform ViewMatrix " << std::endl;
        exit(-1);
    }
    glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, glm::value_ptr(Camera::getInstance()->ViewMatrix));
   
    GLint RotationUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
    if (RotationUniform == -1) 
    {
        std::cerr << "Could not bind uniform ModelMatrix " << std::endl;
        exit(-1);
    }
    glUniformMatrix4fv(RotationUniform, 1, GL_FALSE, glm::value_ptr(ModelMatrix));  

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);

    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vNormals);
    glDisableVertexAttribArray(vUV);
}


//Load BMP file and give it to OPENGL
GLint GraphicsObject::LoadTexture(const std::string & filename)
{
  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename.c_str(), "rb" );

  if ( file == NULL ) return 0;
  width = 256;
  height = 256;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i)
  {
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];

    data[index] = R;
    data[index+2] = B;

  }


  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  
  /* Load texture image into memory */
  glTexImage2D(GL_TEXTURE_2D,     /* Target texture */
		0,                 /* Base level */
		GL_RGB,           /* Each element is RGB triple */ 
		width,    /* Texture dimensions */ 
		height, 
		0,                 /* Border should be zero */
		GL_RGB,            /* Data storage format for BMP file */
		GL_UNSIGNED_BYTE,  /* Type of pixel data, one byte per channel */
		data);    /* Pointer to image data  */

  
  
  /* Next set up texturing parameters */

  /* Repeat texture on edges when tiling */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  /* Linear interpolation for magnification */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* Trilinear MIP mapping for minification */ 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
  
  glGenerateMipmap(GL_TEXTURE_2D);
  
  
  glBindTexture( GL_TEXTURE_2D, 0 );
  free( data );

  return texture;
}
