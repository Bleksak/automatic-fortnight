#include "Input.h"
#include "Camera.h"

#include "GL.h"

void keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window;
    (void) key;
    (void) scancode;
    (void) action; 
    (void) mods;
}
void mouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos)
{
    (void) window;
    (void) xpos;
    (void) ypos;
}
void mouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods)
{
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
    (void) window;
}

void GameKeyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window;
    (void) key;
    (void) scancode;
    (void) action;
    (void) mods;
}

void GameMouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos)
{
    (void) window;
    (void) xpos;
    (void) ypos;
}

void GameMouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods)
{
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
}