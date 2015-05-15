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
#include "GraphicsObject.h"
#include <vector>
#include "objloader.h"
#include "camera.h"
#include "InitClass.h"
#include "glm/glm.hpp"

//set to 1 when using the opensource intel or amd driver
#define USES_MESA_DRIVER 1

/* Strings for loading and storing shader code */
Camera cam;

//storage for all the modles loaded from the obj
std::vector<GraphicsObject> allObjs;

bool MODE1 = true;

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
    glColor3f( 20, 20, 20 );
    glRasterPos2f(10, 10);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
    /* Swap between front and back buffer */ 
    glutSwapBuffers();
}


/******************************************************************
*
* OnIdle
*
*******************************************************************/

void OnIdle()
{
   for(int i = 0; i < allObjs.size(); i ++)
    {
      if(allObjs[i].name.compare("horse") == 1)
      {
	//horses move up and down
	allObjs[i].IdleWork(true);
	continue;
      }
      allObjs[i].IdleWork(false);
    }
    
    if(!MODE1)
      cam.freeFly();
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
* Keyboard
*
* Function to be called on key press in window; set by
* glutKeyboardFunc(); x and y specify mouse position on keypress;
* not used in this example 
*
*******************************************************************/
void Keyboard(unsigned char key, int x, int y)   
{
  if(MODE1)
  {
    switch( key ) 
    {  
      case 'a': cam.move(glm::vec3(0.5,0.0,0.0)); break;
      case 'd': cam.move(glm::vec3(-0.5,0.0,0.0)); break;
      case 's': cam.move(glm::vec3(0.0,0.0,-0.50)); break;
      case 'w': cam.move(glm::vec3(0.0,0.0,0.50)); break;
      case '2': MODE1 = false; cam.ViewMatrix = cam.initialViewMatrix; break;
      default: break;
    
    }
  }
  else
  {
    switch( key )
    {
       case '1': MODE1 = true; break;
       case '+': cam.factor += 0.1; break;
       case '-': cam.factor -= 0.1; break;
       case 'q': GraphicsObject::speed *= 1.1; break;
       case 'e': GraphicsObject::speed /= 1.1; break;
    }
    
  }
    glutPostRedisplay();
}

/******************************************************************
*
* Mouse
*
* Function is called on mouse button press; has been seta
* with glutMouseFunc(), x and y specify mouse coordinates,
* but are not used here.
*
*******************************************************************/

void Mouse(int button, int state, int x, int y) 
{
    if(state == GLUT_DOWN) 
    {
    /* Depending on button pressed, set rotation axis,
      * turn on animation */
      switch(button) 
      {
	  case 3:
		cam.move(glm::vec3(0.0,0.0,1.00));
		break;
	  case 4:
		cam.move(glm::vec3(0.0,0.0,-1.00));
		break;
      }

    }
}

void mouseMovement(int x, int y) 
{   
  if(MODE1)
  {
    int centx = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centy = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    cam.rotate(glm::vec3( (0.001*(y- centy))   , 0.001 * (x - centx) , 0));
    //cam.rotate(glm::vec3());
    if (x != centx || y != centy)
    {
      glutWarpPointer(centx, centy);
    }
  }
}

void acMouseMovement(int x, int y) 
{   
  if(MODE1)
  {
    int centx = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centy = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    cam.rotateScreenMid(glm::vec3( (0.001*(y- centy))   , 0.001 * (x - centx) , 0));
    //cam.rotate(glm::vec3());
    if (x != centx || y != centy)
    {
      glutWarpPointer(centx, centy);
    }
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
    allObjs = ObjLoader::loadObj("res/merry.obj", "res/");
    
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
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(mouseMovement);
    glutMotionFunc(acMouseMovement);
    glutMainLoop();

    
    return EXIT_SUCCESS;
}
