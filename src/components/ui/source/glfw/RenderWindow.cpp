#include "pch.h"
#include "RenderWindow.h"
#include "Defines.h"

namespace UIEngine
{

RenderWindow::RenderWindow(bool enable_vsync, bool enable_msaa) :
    _msaa_enabled(enable_msaa)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    _window = glfwCreateWindow(10, 10, "", NULL, NULL);

    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(enable_vsync ? 1 : 0);

    int samples = 4;
    glGetIntegerv(GL_SAMPLES, &samples);

    if (!samples)
    {
        _msaa_enabled = false;
    }

}

}
