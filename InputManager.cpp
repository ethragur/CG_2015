#include "InputManager.h"
#include "GraphicsObject.h"
#include <GL/freeglut.h>

#include "glm/glm.hpp"
#include "camera.h"


bool InputManager::MODE1 = true;
bool InputManager::LEFT = false;
bool InputManager::RIGHT = false;
bool InputManager::FORWARD = false;
bool InputManager::BACK = false;

/******************************************************************
*
* Keyboard
*
* Function to be called on key press in window; set by
* glutKeyboardFunc(); x and y specify mouse position on keypress;
* not used in this example 
*
*******************************************************************/
void InputManager::keyboard(unsigned char key, int x, int y)   
{
  if(MODE1)
  {
    switch( key ) 
    {  
      case 'a': LEFT = true; break;
      case 'd': RIGHT = true; break;
      case 's': BACK = true; break;
      case 'w': FORWARD = true; break;
      case '2': MODE1 = false; Camera::getInstance()->ViewMatrix = Camera::getInstance()->initialViewMatrix; break;
      default: break;  
    }
  }
  else
  {
    switch( key )
    {
       case '1': MODE1 = true; break;
       case '+': Camera::getInstance()->factor += 0.1; break;
       case '-': Camera::getInstance()->factor -= 0.1; break;
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

void InputManager::mouse(int button, int state, int x, int y) 
{
    if(state == GLUT_DOWN) 
    {
    /* Depending on button pressed, set rotation axis,
      * turn on animation */
      switch(button) 
      {
	  case 3:
		Camera::getInstance()->move(glm::vec3(0.0,0.0,1.00));
		break;
	  case 4:
		Camera::getInstance()->move(glm::vec3(0.0,0.0,-1.00));
		break;
      }

    }
}

void InputManager::mouseMovement(int x, int y) 
{   
  if(MODE1)
  {
    int centx = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centy = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    Camera::getInstance()->rotate(glm::vec3( (0.001*(y- centy))   , 0.001 * (x - centx) , 0));
    //cam.rotate(glm::vec3());
    if (x != centx || y != centy)
    {
      glutWarpPointer(centx, centy);
    }
  }
}

void InputManager::acMouseMovement(int x, int y) 
{   
  if(MODE1)
  {
    int centx = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centy = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    Camera::getInstance()->rotateScreenMid(glm::vec3( (0.001*(y- centy))   , 0.001 * (x - centx) , 0));
    //cam.rotate(glm::vec3());
    if (x != centx || y != centy)
    {
      glutWarpPointer(centx, centy);
    }
  }
}


void InputManager::keyboardUp(unsigned char key, int x, int y)
{
  if(MODE1)
    {
      switch( key ) 
      {  
	case 'a': LEFT = false; break;
	case 'd': RIGHT = false; break;
	case 's': BACK = false; break;
	case 'w': FORWARD = false; break;
	default: break;  
      }
    }
}



void InputManager::onIdle()
{
  if(FORWARD)
  {
    Camera::getInstance()->move(glm::vec3(0,0,0.1));
  }
  else if(BACK)
  {
    Camera::getInstance()->move(glm::vec3(0,0,-0.1));
  }
  else if(LEFT)
  {
    Camera::getInstance()->move(glm::vec3(0.1,0,0));
  }
  else if(RIGHT)
  {
    Camera::getInstance()->move(glm::vec3(-0.1,0,0));
  }
}