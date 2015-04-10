/******************************************************************
*
* RotatingCube.c
*
* Description: This example demonstrates a colored, rotating
* cube in shader-based OpenGL. The use of transformation
* matrices, perspective projection, and indexed triangle sets 
* are shown.
*
* Computer Graphics Proseminar SS 2015
* 
* Interactive Graphics and Simulation Group
* Institute of Computer Science
* University of Innsbruck
*
*******************************************************************/


/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>


/* Local includes */
#include "LoadShader.h"   /* Provides loading function for shader code */
#include "Matrix.h"  
#include "GraphicsObject.h"
#include <vector>


/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

float ProjectionMatrix[16]; /* Perspective projection matrix */
float ViewMatrix[16]; /* Camera view matrix */ 
/*float ModelMatrix[16]; /* Model matrix */ 

GraphicsObject *n;
GraphicsObject *w;
GraphicsObject *pillar;
GraphicsObject *platform1;
GraphicsObject *platform2;

//cube
std::vector<GLfloat> vertex_buffer_data = 
  { /* /* 8 cube vertices XYZ */
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
  };
//pillar  
std::vector<GLfloat> vertex_buffer_data1 = 
  { /* /* 8 cube vertices XYZ */
    -0.2, -4.0,  0.2,
     0.2, -4.0,  0.2,
     0.2,  4.0,  0.2,
    -0.2,  4.0,  0.2,
    -0.2, -4.0, -0.2,
     0.2, -4.0, -0.2,
     0.2,  4.0, -0.2,
    -0.2,  4.0, -0.2,
  };
  
//platform
std::vector<GLfloat> vertex_buffer_data2 = 
  { /* /* 8 cube vertices XYZ */
    -8.0, -0.2,  8.0,
     8.0, -0.2,  8.0,
     8.0,  0.2,  8.0,
    -8.0,  0.2,  8.0,
    -8.0, -0.2, -8.0,
     8.0, -0.2, -8.0,
     8.0,  0.2, -8.0,
    -8.0,  0.2, -8.0,
  };

std::vector<GLfloat>   color_buffer_data = 
  { /* RGB color values for 8 vertices  */
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
  }; 
  
  std::vector<GLfloat>   color_buffer_data1 = 
  { /* RGB color values for 8 vertices  */
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
  }; 


std::vector<GLushort> index_buffer_data = 
  { /* Indices of 6*2 triangles (6 sides) */
      0, 1, 2,
      2, 3, 0,
      1, 5, 6,
      6, 2, 1,
      7, 6, 5,
      5, 4, 7,
      4, 0, 3,
      3, 7, 4,
      4, 5, 1,
      1, 0, 4,
      3, 2, 6,
      6, 7, 3,
  };


/*----------------------------------------------------------------*/


/******************************************************************
*
* Display
*
* This function is called when the content of the window needs to be
* drawn/redrawn. It has been specified through 'glutDisplayFunc()';
* Enable vertex attributes, create binding between C program and 
* attribute name in shader
*
*******************************************************************/

void Display()
{
    /* Clear window; color specified in 'Initialize()' */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    n->Draw(ShaderProgram, ProjectionMatrix, ViewMatrix);
    w->Draw(ShaderProgram, ProjectionMatrix, ViewMatrix);
    pillar->Draw(ShaderProgram, ProjectionMatrix, ViewMatrix);
    platform1->Draw(ShaderProgram, ProjectionMatrix, ViewMatrix);
    platform2->Draw(ShaderProgram, ProjectionMatrix, ViewMatrix);
    /* Swap between front and back buffer */ 
    glutSwapBuffers();
}


/******************************************************************
*
* OnIdle
*
* 
*
*******************************************************************/

void OnIdle()
{
    /*float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (360.0/M_PI); 
    float RotationMatrixAnim[16];

    /* Time dependent rotation 
    SetRotationY(angle, RotationMatrixAnim);
    
    /* Apply model rotation; finally move cube down 
    MultiplyMatrix(RotationMatrixAnim, n->InitialTransform, n->ModelMatrix);
    
    MultiplyMatrix(RotationMatrixAnim, w->InitialTransform, w->ModelMatrix);
    
    
    
     Request redrawing forof window content */  
    w->IdleWork(true);
    n->IdleWork(true);
    pillar->IdleWork(false);
    platform1->IdleWork(false);
    platform2->IdleWork(false);
    glutPostRedisplay();
}



/******************************************************************
*
* AddShader
*
* This function creates and adds individual shaders
*
*******************************************************************/

void AddShader(GLuint ShaderProgram, const char* ShaderCode, GLenum ShaderType)
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


/******************************************************************
*
* CreateShaderProgram
*
* This function creates the shader program; vertex and fragment
* shaders are loaded and linked into program; final shader program
* is put into the rendering pipeline 
*
*******************************************************************/

void CreateShaderProgram()
{
    /* Allocate shader object */
    ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) 
    {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    /* Load shader code from file */
    VertexShaderString = LoadShader("../vertexshader.vs");
    FragmentShaderString = LoadShader("../fragmentshader.fs");

    /* Separately add vertex and fragment shader to program */
    AddShader(ShaderProgram, VertexShaderString, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, FragmentShaderString, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024];

    /* Link shader code into executable shader program */
    glLinkProgram(ShaderProgram);

    /* Check results of linking step */
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

    if (Success == 0) 
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    /* Check if shader program can be executed */ 
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);

    if (!Success) 
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    /* Put linked shader program into drawing pipeline */
    glUseProgram(ShaderProgram);
}


/******************************************************************
*
* Initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/

void Initialize(void)
{   
    /* Set background (clear) color to dark blue */ 
    glClearColor(0.4, 0.4, 0.4, 0.0);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    

    /* Setup vertex, color, and index buffer objects */
    
    /* Setup shaders and shader program */
    CreateShaderProgram();  

    /* Initialize matrices */
    SetIdentityMatrix(ProjectionMatrix);
    SetIdentityMatrix(ViewMatrix);
    
    /* Set projection transform */
    float fovy = 45.0;
    float aspect = 1.0; 
    float nearPlane = 1.0; 
    float farPlane = 100.0;
    SetPerspectiveMatrix(fovy, aspect, nearPlane, farPlane, ProjectionMatrix);

    /* Set viewing transform */
    float camera_disp = -42.0;
    SetTranslation(0.0, 0.0, camera_disp, ViewMatrix);
    
    
    n->initobj(8,0,0);
    w->initobj(-8,0,0);
    pillar->initobj(0,0,0);
    platform1->initobj(0,4,0);
    platform2->initobj(0,-4,0);
   
}


/******************************************************************
*
* main
*
* Main function to setup GLUT, GLEW, and enter rendering loop
*
*******************************************************************/

int main(int argc, char** argv)
{
    n = new GraphicsObject(vertex_buffer_data, color_buffer_data1, index_buffer_data);
    w = new GraphicsObject(vertex_buffer_data, color_buffer_data1, index_buffer_data);
    pillar = new GraphicsObject(vertex_buffer_data1, color_buffer_data, index_buffer_data); 
    platform1 = new GraphicsObject(vertex_buffer_data2, color_buffer_data, index_buffer_data); 
    platform2 = new GraphicsObject(vertex_buffer_data2, color_buffer_data, index_buffer_data); 
    /* Initialize GLUT; set double buffered window and RGBA color model */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("CG Proseminar - Rotating Cube");

    /* Initialize GL extension wrangler */
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    /* Setup scene and rendering parameters */
    Initialize();


    /* Specify callback functions;enter GLUT event processing loop, 
     * handing control over to GLUT */
    glutIdleFunc(OnIdle);
    glutDisplayFunc(Display);
    glutMainLoop();

    
    return EXIT_SUCCESS;
}
