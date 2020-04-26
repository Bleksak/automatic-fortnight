#ifndef _WINDOW_H
#define _WINDOW_H

#include "GL.h"

struct GLOption window_create(const char* title, GLFWmonitor* monitor, const GLFWvidmode* mode, bool fullscreen);
void window_destroy(GLFWwindow* wnd);

#endif
