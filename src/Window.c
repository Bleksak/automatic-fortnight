#include "Window.h"
#include "Input.h"

static void framebuffer_size_callback(GLFWwindow* wnd, int width, int height)
{
    (void) wnd;
    glViewport(0, 0, width, height);
}


struct GLOption GameCreateWindow(const char* wndTitle, GLFWmonitor* monitor, const GLFWvidmode* mode, bool fullscreen)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_FOCUSED, true);
    glfwWindowHint(GLFW_AUTO_ICONIFY, true);
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    glfwWindowHint(GLFW_MAXIMIZED, true);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* wnd;

    if(fullscreen)
    {
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        wnd = glfwCreateWindow(mode->width, mode->height, wndTitle, monitor, 0);
    }
    else
    {
        glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
        wnd = glfwCreateWindow(mode->width, mode->height, wndTitle, 0, 0);
    }

    if(!wnd)
    {
        return (struct GLOption)
        {
            .ok = false,
            .error_message = "Failed to create window",
        };
    }

    glfwMakeContextCurrent(wnd);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return (struct GLOption)
        {
            .ok = false,
            .error_message = "Failed to create window",
        };
    }

    glViewport(0, 0, mode->width, mode->height);
    glfwSetFramebufferSizeCallback(wnd, framebuffer_size_callback);

    glfwShowWindow(wnd);
    glfwFocusWindow(wnd);

    return (struct GLOption)
    {
        .ok = true,
        .result_ptr = wnd
    };
}

void GLDestroyWindow(GLFWwindow* wnd)
{
    glfwDestroyWindow(wnd);
}