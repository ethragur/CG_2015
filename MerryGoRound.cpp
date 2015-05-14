/******************************************************************
* 
* 
* 
* Computer Graphics Project 2015
* 
* by Stefan Kuhnert, Philipp Pobitzer, Tobias Raggl
* 
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
#include "Matrix.h"  
#include "GraphicsObject.h"
#include <vector>
#include "objloader.h"
#include "camera.h"
#include "InitClass.h"

//set to 1 when using the opensource intel or amd driver
#define USES_MESA_DRIVER 1

/* Strings for loading and storing shader code */
Camera cam;

float ProjectionMatrix[16];
float ViewMatrix[16];

//storage for all the modles loaded from the obj
std::vector<GraphicsObject> allObjs;

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
     for(int i = 0; i < allObjs.size(); i ++)
    {
      allObjs[i].Draw(InitClass::ShaderProgram, cam.ProjectionMatrix, cam.ViewMatrix);
    }

    /* Swap between front and back buffer */ 
    glutSwapBuffers();
}


/******************************************************************
*
* OnIdle
* 
* 
* 
* 
*
*******************************************************************/

void OnIdle()
{
   for(int i = 0; i < allObjs.size(); i ++)
    {
      if(allObjs[i].name.compare("horse") == 1)
      {
	allObjs[i].IdleWork(true);
	continue;
      }
      allObjs[i].IdleWork(false);
    }

    glutPostRedisplay();
}


/******************************************************************
*
* Keyboard
*
* Function to be called on key press in window; set by
* glutKeyboardFunc(); x and y specify mouse position on keypress;
* not used in this example 
*
*******************************************************************/

void Keyboard(unsigned char key, int x, int y)   
{
  float rotateCamMat[16];
  SetIdentityMatrix(rotateCamMat);
    switch( key ) 
    {
      
    
    }

    glutPostRedisplay();
}

void Mouse()
{
  
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
    /* Set background (clear) color to grey */ 
    glClearColor(0.4, 0.4, 0.4, 0.0);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    
    /*Intel troubleshooting*/
    InitClass::setupArrayObject();

    
    
    /* Setup shaders and shader program */
    InitClass::CreateShaderProgram();  
    /* Initialize Camera */
    cam = Camera();
    
    
    
    /* Init all Objects in the Obj file */
    for(int i = 0; i < allObjs.size(); i ++)
    {
      allObjs[i].initobj(0,0,0);
    }
  
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
   
    /* Initialize GLUT; set double buffered window and RGBA color model */
    glutInit(&argc, argv);
    InitClass::USES_MESA = (USES_MESA_DRIVER == 1) ? 1 : 0;
    allObjs = ObjLoader::loadObj("merry.obj");
    
    InitClass::setupIntelMesaConfiguration();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("CG Proseminar - MerryGoRound");

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
    glutKeyboardFunc(Keyboard); 
    glutMainLoop();

    
    return EXIT_SUCCESS;
}
