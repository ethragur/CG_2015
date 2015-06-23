#include "GraphicsObject.h"
#include "glm/ext.hpp"
#include <cmath>
#include "SOIL/SOIL.h"
#include <iostream>


float GraphicsObject::speed = 1.0;
bool GraphicsObject::disableSpec = false;
bool GraphicsObject::disableDiff = false;
bool GraphicsObject::disableAmbient = false;
bool GraphicsObject::disableNormalMapping = false;

GraphicsObject::GraphicsObject(
  const std::vector<GLfloat> & vertex_buffer_temp, 
  const std::vector<GLuint> & index_buffer_temp,
  const glm::vec3 & diff_tmp, 
  const glm::vec3 & spec_tmp,
  const std::vector<GLfloat> & tex_tmp,
  GLfloat shiny,
  const std::string &name,
  const std::string &texname,
  const std::string &normtexname) :
vertex_buffer_data(vertex_buffer_temp),
index_buffer_data(index_buffer_temp),
diffuse(diff_tmp),
specular(spec_tmp),
texture_vertex_data(tex_tmp),
shininess(shiny),
name(name),
texname(texname),
normtexname(normtexname)
{ 
  //calculations of vertex normals, tangents and bitangents
  std::vector<glm::vec3> norm(vertex_buffer_temp.size() / 3, glm::vec3(0,0,0));
  std::vector<glm::vec3> tangents(vertex_buffer_temp.size() / 3, glm::vec3(0,0,0));
  std::vector<glm::vec3> bitangents(vertex_buffer_temp.size() / 3, glm::vec3(0,0,0));
  std::vector<GLfloat> tmp_tangents(vertex_buffer_temp.size());
 //std::vector<GLfloat> tmp_bitangents(vertex_buffer_temp.size());
  
  
  std::cout << "Calculating Normals and Tangents for: " << name << std::endl;
  for(int i = 0; i < index_buffer_temp.size() ; i+=3)
  {
    
    glm::vec3 v1 = glm::vec3(vertex_buffer_temp[index_buffer_temp[i+0]*3+0], vertex_buffer_temp[index_buffer_temp[i+0]*3+1],vertex_buffer_temp[index_buffer_temp[i+0]*3+2]);
    glm::vec3 v2 = glm::vec3(vertex_buffer_temp[index_buffer_temp[i+1]*3+0], vertex_buffer_temp[index_buffer_temp[i+1]*3+1],vertex_buffer_temp[index_buffer_temp[i+1]*3+2]);
    glm::vec3 v3 = glm::vec3(vertex_buffer_temp[index_buffer_temp[i+2]*3+0], vertex_buffer_temp[index_buffer_temp[i+2]*3+1],vertex_buffer_temp[index_buffer_temp[i+2]*3+2]);
    
    glm::vec2 texCoordv1 = glm::vec2(texture_vertex_data[index_buffer_temp[i+0]*2+0], texture_vertex_data[index_buffer_temp[i+0]*2+1]);
    glm::vec2 texCoordv2 = glm::vec2(texture_vertex_data[index_buffer_temp[i+1]*2+0], texture_vertex_data[index_buffer_temp[i+1]*2+1]);
    glm::vec2 texCoordv3 = glm::vec2(texture_vertex_data[index_buffer_temp[i+2]*2+0], texture_vertex_data[index_buffer_temp[i+2]*2+1]);
    
    glm::vec3 fn = glm::normalize(glm::cross(v3-v2,v1-v2));
    
    norm[(index_buffer_temp[i+0])] += fn; 
    norm[(index_buffer_temp[i+1])] += fn;
    norm[(index_buffer_temp[i+2])] += fn;
    
    glm::vec3 tangent;
    
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v3 - v2;
    
    float DeltaU1 = texCoordv2.x - texCoordv1.x;
    float DeltaV1 = texCoordv2.y - texCoordv1.y;
    float DeltaU2 = texCoordv3.x - texCoordv1.x;
    float DeltaV2 = texCoordv3.y - texCoordv1.y;
    
    float f = 1.0f / (DeltaU1 * DeltaV2 -DeltaU2 * DeltaV1);
    
    tangent = glm::vec3( f * (DeltaV2 * edge1.x - DeltaV1 * edge2.x), f * (DeltaV2 * edge1.y - DeltaV1 * edge2.y) ,f * (DeltaV2 * edge1.z - DeltaV1 * edge2.z));
    
    //bitangent = glm::vec3 (f * (-DeltaU2 * edge1.x - DeltaU1 * edge2.x), f * (-DeltaU2 * edge1.y - DeltaU1 * edge2.y), f * (-DeltaU2 * edge1.z - DeltaU1 * edge2.z));
    
    tangents[(index_buffer_temp[i+0])] += tangent; 
    tangents[(index_buffer_temp[i+1])] += tangent;
    tangents[(index_buffer_temp[i+2])] += tangent;
    
    /*
    bitangents[(index_buffer_temp[i+0])] += bitangent; 
    bitangents[(index_buffer_temp[i+1])] += bitangent;
    bitangents[(index_buffer_temp[i+2])] += bitangent;
    */
    
  }
  
  std::vector<GLfloat> resNorm(vertex_buffer_temp.size(), 0);
  std::vector<GLfloat> resTang(vertex_buffer_temp.size(), 0);
  //std::vector<GLfloat> biresTang(vertex_buffer_temp.size(), 0);
  
  for(int i = 0; i < norm.size(); i++)
  {
    resNorm[i*3+0] = norm[i].x;
    resNorm[i*3+1] = norm[i].y;
    resNorm[i*3+2] = norm[i].z;
    
    resTang[i*3+0] = tangents[i].x;
    resTang[i*3+1] = tangents[i].y;
    resTang[i*3+2] = tangents[i].z;
    /*
    biresTang[i*3+0] = bitangents[i].x;
    biresTang[i*3+1] = bitangents[i].y;
    biresTang[i*3+2] = bitangents[i].z;
    */
  }
  
  vertex_normal_data = resNorm;
  vertex_tangent_data = resTang;
 // vertex_bintangent_data = biresTang;
  //calc position
  GLfloat x = 0.0f;
  GLfloat y = 0.0f;
  GLfloat z = 0.0f;
  for(int i = 0; i < vertex_buffer_temp.size(); i = i+3)
  {
    x  += vertex_buffer_temp[i];
    y  += vertex_buffer_temp[i+1];
    z  += vertex_buffer_temp[i+2];
  }
  
  position = glm::vec3(x / (vertex_buffer_temp.size() /3), y / (vertex_buffer_temp.size() /3), z / (vertex_buffer_temp.size() / 3));
 
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
  
  std::string texpath;
  std::string normtexpath;
  
  //load texture from materials
  if(texname.size() > 0)
  {
    texpath = "../res/" + texname;
  }
  else
  {
    texpath = "../res/default.png";
  }
  
  if(normtexname.size() > 0)
  {
    normtexpath = "../res/" + normtexname;
  }
  else
  {
    normtexpath = "../res/default_normal.png";
  }
  
  textureID = LoadTexture(texpath);
  normalMapID = LoadTexture(normtexpath); 
}


void GraphicsObject::IdleWork(bool updown)
{
  //compute FPS independent TIME
  oldtime = newtime;
  newtime = glutGet(GLUT_ELAPSED_TIME) ; 
  if(!(name.compare("back1") == 0 || name.compare("Plane") == 0 || name.compare("Billboard") <= 1))
  {
    rotAroundCenter();
  
    if(updown)
    {
      UpDown();
    }
  } 
  
  //Get all the objects the start with Billboard
  if(name.find("Billboard") != std::string::npos)
  {
	//std::cout << name << std::endl;
    	glm::vec3 viewDir = glm::vec3(0, 0, 1);
	
	glm::vec3 dist = Camera::getInstance()->Position - position;
	dist.y = 0;
	dist = glm::normalize(dist);
	glm::vec3 up = glm::cross(viewDir, dist);
	GLfloat alpha = glm::dot(viewDir, dist);
	ModelMatrix = glm::translate(position);
	ModelMatrix *= glm::rotate((GLfloat)acos(alpha), up);
	ModelMatrix *= glm::translate(-position);
  }
  
}


/* Updown Movement Function for the Horses
 * string comp not really a good solution...
 * 
 * */
void GraphicsObject::UpDown()
{
  if(name == "horse1" || name == "horse2")
  {
    TranslateOrigin = glm::translate(glm::vec3(0,cos(((newtime)/10)*(M_PI/180))/1.2f,0));
    ModelMatrix = TranslateOrigin * ModelMatrix;
  }
  else
  {
    TranslateOrigin = glm::translate(glm::vec3(0,sin(((newtime)/10)*(M_PI/180))/1.2f,0));
    ModelMatrix = TranslateOrigin * ModelMatrix;
  }
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
 
    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_tangent_data.size() * sizeof(GLfloat), &vertex_tangent_data[0], GL_STATIC_DRAW);
    
    /*
    ** calculated in Vertex shader
    glGenBuffers(1, &BBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_bintangent_data.size() * sizeof(GLfloat), &vertex_bintangent_data, GL_STATIC_DRAW);
    */
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
    
    glEnableVertexAttribArray(vTangents);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(vTangents,3, GL_FLOAT, GL_FALSE, 0,0);
    
    /* calc done in Vertex shader
    glEnableVertexAttribArray(vBiTangents);
    glBindBuffer(GL_ARRAY_BUFFER, BBO);
    glVertexAttribPointer(vBiTangents,3, GL_FLOAT, GL_FALSE,0,0);
    */
    
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    GLint size; 
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);  

    //create Lightning vectors for FragmentShader
    std::vector<GLfloat> lightPositionMat(lightSources.size() * 3, 0);
    std::vector<GLfloat> lightColorMat(lightSources.size() * 3, 0);
    std::vector<GLfloat> lightIntensity(lightSources.size(), 0);
    
    //fill those vectors
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
  
    /*
     * Not needed in Viewspace only in World
    GLint cameraPosShader = glGetUniformLocation(ShaderProgram, "cameraPos"); 
    glUniform3f(cameraPosShader, Camera::getInstance()->cameraPos.x, Camera::getInstance()->cameraPos.y, Camera::getInstance()->cameraPos.z);
    */
    
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
    
    GLint normalMapping = glGetUniformLocation(ShaderProgram, "disableNormalMapping");
    glUniform1i(normalMapping, GraphicsObject::disableNormalMapping);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMapID);
    GLint normTexSampler = glGetUniformLocation(ShaderProgram, "normTexSampler");
    glUniform1i(normTexSampler, 1);
    
    
    
    
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
    glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, glm::value_ptr(Camera::getInstance()->GetViewMatrix()));
   
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
    glDisableVertexAttribArray(vTangents);
    //glDisableVertexAttribArray(vBiTangents);
}


//Load BMP file and give it to OPENGL
GLint GraphicsObject::LoadTexture(const std::string & filename)
{
  std::cout << "Loading Texture: " << filename << " for: " << name << std::endl;
  GLuint texture;

  int width, height;

  unsigned char * data;
  
  
  data = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA );

  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  
  /* Load texture image into memory */
  glTexImage2D(GL_TEXTURE_2D,     /* Target texture */
		0,                 /* Base level */
		GL_RGBA,           /* Each element is RGB triple */ 
		width,    /* Texture dimensions */ 
		height, 
		0,                 /* Border should be zero */
		GL_RGBA,            /* Data storage format for BMP file */
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
  
  
  free( data );

  return texture;
}
