#ifndef _WINDOW_H
#define _WINDOW_H

#include "GL.h"

struct GLOption GameCreateWindow(const char* wndTitle, GLFWmonitor* monitor, const GLFWvidmode* mode, bool fullscreen);
void GLDestroyWindow(GLFWwindow* wnd);

#endif