/******************************************************************* 
* 
* Computer Graphics Project 2015
* 
* by Stefan Kuhnert, Philipp Pobitzer, Felix Putz
* 
*******************************************************************/

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>

/* Local includes */
#include "GraphicsObject.h"
#include "objloader.h"
#include "camera.h"
#include "InitClass.h"
#include "InputManager.h"
#include "LightSource.h"

#include "glm/gtx/string_cast.hpp"

//set to 1 when using the opensource intel or amd driver
#define USES_MESA_DRIVER 0

//storage for all the modles loaded from the obj
std::vector<GraphicsObject> allObjs;
std::vector<LightSource> lightSources;

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
      allObjs[i].Draw(InitClass::ShaderProgram, lightSources);
    }
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
  for(int i = 0; i < allObjs.size(); i++)
  {
    
    if(allObjs[i].name[0] == 'h')
    {
      allObjs[i].IdleWork(true);
     
      continue;
    }
    allObjs[i].IdleWork(false);
  }
  
  for(int i = 0; i < lightSources.size(); i++)
  {
    if(lightSources[i].moving)
    {
      lightSources[i].move();
    }
      
  }
  InputManager::onIdle();
  glutPostRedisplay();
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
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    /*Intel troubleshooting*/
    InitClass::setupArrayObject();    
    
    /* Setup shaders and shader program */
    InitClass::CreateShaderProgram();  
    
    /* Init all Objects in the Obj file */
    
    for(int i = 0; i < allObjs.size(); i ++)
    {
      allObjs[i].initobj(0,0,0);
      
      if(allObjs[i].name.compare("Light1") == 0)
      {
	 LightSource tmp(glm::vec3(allObjs[i].position.x,allObjs[i].position.y,allObjs[i].position.z) , glm::vec3(1,1,1), false);
	 lightSources.push_back(tmp);
      }
      if(allObjs[i].name.compare("Light2") == 0)
      { 
	 LightSource tmp(glm::vec3(allObjs[i].position.x,allObjs[i].position.y,allObjs[i].position.z), glm::vec3(1,1,1), true);
	 lightSources.push_back(tmp);
      }
      if(allObjs[i].name.compare("Light3") == 0)
      {
 	 LightSource tmp(glm::vec3(allObjs[i].position.x,allObjs[i].position.y,allObjs[i].position.z), glm::vec3(1,1,1), true);
	 lightSources.push_back(tmp);
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
    allObjs = ObjLoader::loadObj("../res/merry.obj", "../res/");
    InitClass::setupIntelMesaConfiguration();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("CG Proseminar - MerryGoRound");
    //enable FSAA
    glEnable(GL_MULTISAMPLE);

    /* Initialize GL extension wrangler */
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
	std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        return 1;
    }

    /* Setup scene and rendering parameters */
    Initialize();

    /* Specify callback functions;enter GLUT event processing loop, 
     * handing control over to GLUT */
    glutIdleFunc(OnIdle);
    glutDisplayFunc(Display);
    glutKeyboardFunc(InputManager::keyboard); 
    glutKeyboardUpFunc(InputManager::keyboardUp);
    glutMouseFunc(InputManager::mouse);
    glutPassiveMotionFunc(InputManager::mouseMovement);
    glutMotionFunc(InputManager::acMouseMovement);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
