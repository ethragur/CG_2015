#include "InitClass.h"


//definitions of static vars
int InitClass::USES_MESA = 0;
GLuint InitClass::VAO = 0;
const char* InitClass::VertexShaderString = NULL;
const char* InitClass::FragmentShaderString = NULL;
GLuint InitClass::ShaderProgram = 0;

/******************************************************************
*
* CreateShaderProgram
*
* This function creates the shader program; vertex and fragment
* shaders are loaded and linked into program; final shader program
* is put into the rendering pipeline 
*
*******************************************************************/
void InitClass::CreateShaderProgram()
{
  /* Allocate shader object */
  InitClass::ShaderProgram = glCreateProgram();

  if (InitClass::ShaderProgram == 0) 
  {
      fprintf(stderr, "Error creating shader program\n");
      exit(1);
  }

  /* Load shader code from file */
  InitClass::VertexShaderString = InitClass::LoadShader("vertexshader.vs");
  InitClass::FragmentShaderString = InitClass::LoadShader("fragmentshader.fs");

  /* Separately add vertex and fragment shader to program */
  InitClass::AddShader(InitClass::ShaderProgram, VertexShaderString, GL_VERTEX_SHADER);
  InitClass::AddShader(InitClass::ShaderProgram, FragmentShaderString, GL_FRAGMENT_SHADER);

  GLint Success = 0;
  GLchar ErrorLog[1024];

  /* Link shader code into executable shader program */
  glLinkProgram(InitClass::ShaderProgram);

  /* Check results of linking step */
  glGetProgramiv(InitClass::ShaderProgram, GL_LINK_STATUS, &Success);

  if (Success == 0) 
  {
      glGetProgramInfoLog(InitClass::ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
      fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
      exit(1);
  }

  /* Check if shader program can be executed */ 
  glValidateProgram(InitClass::ShaderProgram);
  glGetProgramiv(InitClass::ShaderProgram, GL_VALIDATE_STATUS, &Success);

  if (!Success) 
  {
      glGetProgramInfoLog(InitClass::ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
      fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
      exit(1);
  }

  /* Put linked shader program into drawing pipeline */
  glUseProgram(InitClass::ShaderProgram);
}




/******************************************************************
*
* LoadShader
*
* This function reads and returns a string from the file 'filename';
* it is used to load the shader source code
*
*******************************************************************/
const char* InitClass::LoadShader(const char* filename)
{
#ifdef WIN32
    FILE* infile;
    fopen_s(&infile, filename, "rb");
#else
    FILE* infile = fopen(filename, "rb");
#endif // WIN32

    if (!infile) 
    {
        fprintf(stderr, "Could not open shader file %s\n", filename);
        exit(0);
    }

    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    char* source = (char*)malloc(sizeof(char) * (len+1));

    fread(source, 1, len, infile);
    fclose(infile);

    source[len] = 0;

    return (const char*)(source);
}



/******************************************************************
*
* AddShader
*
* This function creates and adds individual shaders
*
*******************************************************************/
void InitClass::AddShader(GLuint ShaderProgram, const char* ShaderCode, GLenum ShaderType)
{
  /* Create shader object */
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
      fprintf(stderr, "Error creating shader type %d\n", ShaderType);
      exit(0);

  }

  /* Associate shader source code string with shader object */
  glShaderSource(ShaderObj, 1, &ShaderCode, NULL);

  GLint success = 0;
  GLchar InfoLog[1024];

  /* Compile shader source code */
  glCompileShader(ShaderObj);
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
      glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
      fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
      exit(1);
  }

  /* Associate shader with shader program */
  glAttachShader(ShaderProgram, ShaderObj);
}


void InitClass::setupArrayObject()
{
    if (InitClass::USES_MESA)
    {
      glGenVertexArrays(1, &InitClass::VAO); // Create our Vertex Array Object  
      glBindVertexArray(InitClass::VAO); // Bind our Vertex Array Object so we can use it 
    }
}

void InitClass::setupIntelMesaConfiguration()
{

	if (InitClass::USES_MESA)
	{
	  //INTEL MESA TROUBLESHOOTING
	  glutInitContextVersion(3,3);
	  glutInitContextProfile(GLUT_CORE_PROFILE);
	  glewExperimental = GL_TRUE;
	}
}
