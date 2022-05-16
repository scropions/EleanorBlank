#pragma once
#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/platform/Config.h>

typedef struct GLFWwindow GLFWwindow;

#define ENABLE_OFFSCREEN_GL 0

namespace UIEngine
{

class RenderWindow
{

public:
    RenderWindow() = delete;
    RenderWindow(bool enable_vsync, bool enable_msaa);

    virtual ~RenderWindow() {}

    GLFWwindow* window() { return _window; }

protected:
    GLFWwindow* _window;
    bool _msaa_enabled;
};

}