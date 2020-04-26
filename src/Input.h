#pragma once

#include "GL.h"
#include <stdbool.h>

void keyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods);

void GameKeyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
void GameMouseMoveCallbackFun(GLFWwindow* window, double xpos, double ypos);
void GameMouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods);

void game_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void game_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void game_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void input_update(struct GL* gl);
