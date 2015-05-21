#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


class InputManager
{
  
private:
  //vars
  static bool FORWARD;
  static bool BACK;
  static bool RIGHT;
  static bool LEFT;
  static bool MODE1;
  
  
public:
  //func
  static void keyboard(unsigned char key, int x, int y);
  static void keyboardUp(unsigned char key, int x, int y);
  static void mouse(int button, int state, int x, int y);
  static void mouseMovement(int x, int y);
  static void acMouseMovement(int x, int y);
  static void onIdle();
  
};


#endif //INPUTMANAGER_H
