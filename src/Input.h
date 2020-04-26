#include "GL.h"

#ifndef _INPUT_H
#define _INPUT_H

void keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods);

void GameKeyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
void GameMouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos);
void GameMouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods);

#endif
