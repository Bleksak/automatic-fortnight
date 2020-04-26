#pragma once
#include "GL.h"

struct GLOption window_create(const char* title, GLFWmonitor* monitor, const GLFWvidmode* mode, bool fullscreen);
void window_destroy(GLFWwindow* wnd);
