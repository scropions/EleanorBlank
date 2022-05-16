#include "pch.h"
#include "Window.h"
#include "Engine.h"
#include "View.h"
#include "Defines.h"

static int GLFWModsToUltralightMods(int mods);
static int GLFWKeyCodeToUltralightKeyCode(int key);

extern "C" {

    static void Window_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error: %s\n", description);
    }

    static void WindowGLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        ultralight::KeyEvent evt;
        evt.type = action == GLFW_PRESS || action == GLFW_REPEAT ?
                   ultralight::KeyEvent::kType_RawKeyDown : ultralight::KeyEvent::kType_KeyUp;
        evt.virtual_key_code = GLFWKeyCodeToUltralightKeyCode(key);
        evt.native_key_code = scancode;
        ultralight::GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);
        evt.modifiers = GLFWModsToUltralightMods(mods);
        win->OnKeyEvent(evt);
        //
        //if (evt.type == ultralight::KeyEvent::kType_RawKeyDown &&
        //    (key == GLFW_KEY_ENTER || key == GLFW_KEY_TAB)) {
        //    // We have to synthesize the Char Event for these keys.
        //    ultralight::KeyEvent evt;
        //    evt.type = ultralight::KeyEvent::kType_Char;
        //    ultralight::String text = key == GLFW_KEY_ENTER ? ultralight::String("\r") : ultralight::String("\t");
        //    evt.text = text;
        //    evt.unmodified_text = text;
        //    win->FireKeyEvent(evt);
        //}
    }

    static void WindowGLFW_char_callback(GLFWwindow* window, unsigned int codepoint)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        ultralight::KeyEvent evt;
        evt.type = ultralight::KeyEvent::kType_Char;

        ultralight::String text = ultralight::String32((const char32_t*)&codepoint, 1);

        evt.text = text;
        evt.unmodified_text = text;

        win->OnKeyEvent(evt);
    }

    static void WindowGLFW_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        ultralight::MouseEvent evt;
        evt.type = ultralight::MouseEvent::kType_MouseMoved;
        evt.x = win->PixelsToScreen((int)xpos);
        evt.y = win->PixelsToScreen((int)ypos);
        evt.button = ultralight::MouseEvent::kButton_None;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            evt.button = ultralight::MouseEvent::kButton_Left;
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
            evt.button = ultralight::MouseEvent::kButton_Middle;
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            evt.button = ultralight::MouseEvent::kButton_Right;
        win->OnMouseEvent(evt);
    }

    static void WindowGLFW_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        ultralight::MouseEvent evt;
        evt.type = action == GLFW_PRESS ? ultralight::MouseEvent::kType_MouseDown : ultralight::MouseEvent::kType_MouseUp;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        evt.x = win->PixelsToScreen((int)xpos);
        evt.y = win->PixelsToScreen((int)ypos);
        evt.button = ultralight::MouseEvent::kButton_None;
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            evt.button = ultralight::MouseEvent::kButton_Left;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            evt.button = ultralight::MouseEvent::kButton_Middle;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            evt.button = ultralight::MouseEvent::kButton_Right;
            break;
        }
        win->OnMouseEvent(evt);
    }

    static void WindowGLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        ultralight::ScrollEvent evt;
        evt.type = ultralight::ScrollEvent::kType_ScrollByPixel;
        evt.delta_x = win->PixelsToScreen((int)xoffset * 32);
        evt.delta_y = win->PixelsToScreen((int)yoffset * 32);
        win->OnScrollEvent(evt);
    }

    static void WindowGLFW_window_size_callback(GLFWwindow* window, int width, int height)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        win->OnResize((uint32_t)width, (uint32_t)height);
    }

    static void WindowGLFW_focus_callback(GLFWwindow* window, int focused)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        //win->SetWindowFocused((bool)focused);
    }

    static void WindowGLFW_refresh_callback(GLFWwindow* window)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        win->InvalidateWindow();
    }

    static void WindowGLFW_close_callback(GLFWwindow* window)
    {
        UIEngine::UIWindow* win = static_cast<UIEngine::UIWindow*>(glfwGetWindowUserPointer(window));
        // win->OnClose();
    }

} // extern "C"

namespace UIEngine
{
using uint32_t = std::uint32_t;

UIWindow::UIWindow(std::uint32_t width, std::uint32_t height,
                   bool fullscreen, unsigned int window_flags) : _fullscreen(fullscreen)
{
    glfwSetErrorCallback(Window_error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);


    _engine = std::make_unique<Engine>();

    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    _window = glfwCreateWindow(width, height,
                               "Application", NULL, _engine->GetRenderWindow()->window());
    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    _engine->Init(_window);

    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetWindowUserPointer(_window, this);

    glfwSetKeyCallback(_window, WindowGLFW_key_callback);
    glfwSetCharCallback(_window, WindowGLFW_char_callback);
    glfwSetCursorPosCallback(_window, WindowGLFW_cursor_pos_callback);
    glfwSetMouseButtonCallback(_window, WindowGLFW_mouse_button_callback);
    glfwSetScrollCallback(_window, WindowGLFW_scroll_callback);
    glfwSetWindowSizeCallback(_window, WindowGLFW_window_size_callback);
    glfwSetWindowFocusCallback(_window, WindowGLFW_focus_callback);
    glfwSetWindowCloseCallback(_window, WindowGLFW_close_callback);

    _cursor_ibeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    _cursor_crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    _cursor_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    _cursor_hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    _cursor_vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    glfwSwapInterval(1);

    float t_init = (float)glfwGetTime();



    //SetWindowScale(scale());
}

UIWindow::~UIWindow()
{
    glfwSetKeyCallback(_window, nullptr);
    glfwSetCharCallback(_window, nullptr);
    glfwSetCursorPosCallback(_window, nullptr);
    glfwSetMouseButtonCallback(_window, nullptr);
    glfwSetScrollCallback(_window, nullptr);
    glfwSetWindowSizeCallback(_window, nullptr);
    glfwSetWindowFocusCallback(_window, nullptr);

    glfwDestroyCursor(_cursor_ibeam);
    glfwDestroyCursor(_cursor_crosshair);
    glfwDestroyCursor(_cursor_hand);
    glfwDestroyCursor(_cursor_hresize);
    glfwDestroyCursor(_cursor_vresize);

    glfwDestroyWindow(_window);
}

void UIWindow::run()
{
    while (!glfwWindowShouldClose(_window))
    {



        //m_uiEngine->Draw();
        Repaint();
        //m_uiEngine->Update();

        //glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
}

uint32_t UIWindow::screen_width() const
{
    return PixelsToScreen(width());
}

uint32_t UIWindow::width() const
{
    // Returns width in pixels
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    return (uint32_t)width;
}

uint32_t UIWindow::screen_height() const
{
    return PixelsToScreen(height());
}

uint32_t UIWindow::height() const
{
    // Return height in pixels
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    return (uint32_t)height;
}

double UIWindow::scale() const
{
    return 1;
}

void UIWindow::MoveTo(int x, int y)
{
    x = ScreenToPixels(x);
    y = ScreenToPixels(y);
    glfwSetWindowPos(_window, x, y);
}

void UIWindow::MoveToCenter()
{
    // MoveTo(PixelsToScreen(monitor_->width()) / 2 - screen_width() / 2,
    //        PixelsToScreen(monitor_->height()) / 2 - screen_height() / 2);
}

int UIWindow::x() const
{
    int xPos, yPos;
    glfwGetWindowPos(_window, &xPos, &yPos);
    return PixelsToScreen(xPos);
}

int UIWindow::y() const
{
    int xPos, yPos;
    glfwGetWindowPos(_window, &xPos, &yPos);
    return PixelsToScreen(yPos);
}

void UIWindow::SetTitle(const char* title)
{
    glfwSetWindowTitle(_window, title);
}

void UIWindow::SetCursor(ultralight::Cursor cursor)
{
    switch (cursor)
    {
    case ultralight::kCursor_Cross:
        glfwSetCursor(_window, _cursor_crosshair);
        break;
    case ultralight::kCursor_Hand:
        glfwSetCursor(_window, _cursor_hand);
        break;
    case ultralight::kCursor_IBeam:
        glfwSetCursor(_window, _cursor_ibeam);
        break;
    case ultralight::kCursor_EastWestResize:
        glfwSetCursor(_window, _cursor_hresize);
        break;
    case ultralight::kCursor_NorthSouthResize:
        glfwSetCursor(_window, _cursor_vresize);
        break;
    default:
        glfwSetCursor(_window, nullptr);
    }
}

void UIWindow::Show()
{
    glfwShowWindow(_window);
}

void UIWindow::Hide()
{
    glfwHideWindow(_window);
}

bool UIWindow::Visible() const
{
    return glfwGetWindowAttrib(_window, GLFW_VISIBLE);
}

void UIWindow::Close()
{
    glfwSetWindowShouldClose(_window, 1);
}

void* UIWindow::native_handle() const
{
    return _window;
}

void UIWindow::Repaint()
{

    glfwMakeContextCurrent(_engine->GetRenderWindow()->window());

    _engine->Update();
    _engine->Render();

    if (_engine->GetGPUDriver()->HasCommandsPending() ||  _need_repaint)
    {
        glfwMakeContextCurrent(_window);
        _engine->GetGPUDriver()->DrawCommandList();
        _engine->Draw();
        glfwSwapBuffers(_window);
    }

    _need_repaint = false;


}

void UIWindow::OnKeyEvent(const ultralight::KeyEvent& evt)
{
    _engine->GetView()->GetView()->FireKeyEvent(evt);
}

void UIWindow::OnMouseEvent(const ultralight::MouseEvent& evt)
{
    _engine->GetView()->GetView()->FireMouseEvent(evt);
}

void UIWindow::OnScrollEvent(const ultralight::ScrollEvent& evt)
{
    _engine->GetView()->GetView()->FireScrollEvent(evt);
}

void UIWindow::OnResize(int width, int height)
{
    ////m_uiEngine->GetGPUContent()->Resize(width, height);
    //
    //m_driver->SetViewport(width, height);
    //m_uiEngine->GetActiveView()->GetView()->Resize(width, height);
    //
    ////m_uiEngine->updateGeometry();
    //
    //// Update these now since they were invalidated
    //ultralight::RenderTarget target = m_uiEngine->GetActiveView()->GetView()->render_target();
    //m_uiEngine->_gpu_state.texture_1_id = target.texture_id;
    //m_uiEngine->_gpu_state.viewport_width = UIWindow::width() ;
    //m_uiEngine->_gpu_state.viewport_height = UIWindow::height();
}

}

using namespace ultralight;
using namespace ultralight::KeyCodes;

int GLFWModsToUltralightMods(int mods)
{
    int result = 0;
    if (mods & GLFW_MOD_ALT)
        result |= KeyEvent::kMod_AltKey;
    if (mods & GLFW_MOD_CONTROL)
        result |= KeyEvent::kMod_CtrlKey;
    if (mods & GLFW_MOD_SUPER)
        result |= KeyEvent::kMod_MetaKey;
    if (mods & GLFW_MOD_SHIFT)
        result |= KeyEvent::kMod_ShiftKey;
    return result;
}

int GLFWKeyCodeToUltralightKeyCode(int key)
{
    switch (key)
    {
    case GLFW_KEY_SPACE:
        return GK_SPACE;
    case GLFW_KEY_APOSTROPHE:
        return GK_OEM_7;
    case GLFW_KEY_COMMA:
        return GK_OEM_COMMA;
    case GLFW_KEY_MINUS:
        return GK_OEM_MINUS;
    case GLFW_KEY_PERIOD:
        return GK_OEM_PERIOD;
    case GLFW_KEY_SLASH:
        return GK_OEM_2;
    case GLFW_KEY_0:
        return GK_0;
    case GLFW_KEY_1:
        return GK_1;
    case GLFW_KEY_2:
        return GK_2;
    case GLFW_KEY_3:
        return GK_3;
    case GLFW_KEY_4:
        return GK_4;
    case GLFW_KEY_5:
        return GK_5;
    case GLFW_KEY_6:
        return GK_6;
    case GLFW_KEY_7:
        return GK_7;
    case GLFW_KEY_8:
        return GK_8;
    case GLFW_KEY_9:
        return GK_9;
    case GLFW_KEY_SEMICOLON:
        return GK_OEM_1;
    case GLFW_KEY_EQUAL:
        return GK_OEM_PLUS;
    case GLFW_KEY_A:
        return GK_A;
    case GLFW_KEY_B:
        return GK_B;
    case GLFW_KEY_C:
        return GK_C;
    case GLFW_KEY_D:
        return GK_D;
    case GLFW_KEY_E:
        return GK_E;
    case GLFW_KEY_F:
        return GK_F;
    case GLFW_KEY_G:
        return GK_G;
    case GLFW_KEY_H:
        return GK_H;
    case GLFW_KEY_I:
        return GK_I;
    case GLFW_KEY_J:
        return GK_J;
    case GLFW_KEY_K:
        return GK_K;
    case GLFW_KEY_L:
        return GK_L;
    case GLFW_KEY_M:
        return GK_M;
    case GLFW_KEY_N:
        return GK_N;
    case GLFW_KEY_O:
        return GK_O;
    case GLFW_KEY_P:
        return GK_P;
    case GLFW_KEY_Q:
        return GK_Q;
    case GLFW_KEY_R:
        return GK_R;
    case GLFW_KEY_S:
        return GK_S;
    case GLFW_KEY_T:
        return GK_T;
    case GLFW_KEY_U:
        return GK_U;
    case GLFW_KEY_V:
        return GK_V;
    case GLFW_KEY_W:
        return GK_W;
    case GLFW_KEY_X:
        return GK_X;
    case GLFW_KEY_Y:
        return GK_Y;
    case GLFW_KEY_Z:
        return GK_Z;
    case GLFW_KEY_LEFT_BRACKET:
        return GK_OEM_4;
    case GLFW_KEY_BACKSLASH:
        return GK_OEM_5;
    case GLFW_KEY_RIGHT_BRACKET:
        return GK_OEM_6;
    case GLFW_KEY_GRAVE_ACCENT:
        return GK_OEM_3;
    case GLFW_KEY_WORLD_1:
        return GK_UNKNOWN;
    case GLFW_KEY_WORLD_2:
        return GK_UNKNOWN;
    case GLFW_KEY_ESCAPE:
        return GK_ESCAPE;
    case GLFW_KEY_ENTER:
        return GK_RETURN;
    case GLFW_KEY_TAB:
        return GK_TAB;
    case GLFW_KEY_BACKSPACE:
        return GK_BACK;
    case GLFW_KEY_INSERT:
        return GK_INSERT;
    case GLFW_KEY_DELETE:
        return GK_DELETE;
    case GLFW_KEY_RIGHT:
        return GK_RIGHT;
    case GLFW_KEY_LEFT:
        return GK_LEFT;
    case GLFW_KEY_DOWN:
        return GK_DOWN;
    case GLFW_KEY_UP:
        return GK_UP;
    case GLFW_KEY_PAGE_UP:
        return GK_PRIOR;
    case GLFW_KEY_PAGE_DOWN:
        return GK_NEXT;
    case GLFW_KEY_HOME:
        return GK_HOME;
    case GLFW_KEY_END:
        return GK_END;
    case GLFW_KEY_CAPS_LOCK:
        return GK_CAPITAL;
    case GLFW_KEY_SCROLL_LOCK:
        return GK_SCROLL;
    case GLFW_KEY_NUM_LOCK:
        return GK_NUMLOCK;
    case GLFW_KEY_PRINT_SCREEN:
        return GK_SNAPSHOT;
    case GLFW_KEY_PAUSE:
        return GK_PAUSE;
    case GLFW_KEY_F1:
        return GK_F1;
    case GLFW_KEY_F2:
        return GK_F2;
    case GLFW_KEY_F3:
        return GK_F3;
    case GLFW_KEY_F4:
        return GK_F4;
    case GLFW_KEY_F5:
        return GK_F5;
    case GLFW_KEY_F6:
        return GK_F6;
    case GLFW_KEY_F7:
        return GK_F7;
    case GLFW_KEY_F8:
        return GK_F8;
    case GLFW_KEY_F9:
        return GK_F9;
    case GLFW_KEY_F10:
        return GK_F10;
    case GLFW_KEY_F11:
        return GK_F11;
    case GLFW_KEY_F12:
        return GK_F12;
    case GLFW_KEY_F13:
        return GK_F13;
    case GLFW_KEY_F14:
        return GK_F14;
    case GLFW_KEY_F15:
        return GK_F15;
    case GLFW_KEY_F16:
        return GK_F16;
    case GLFW_KEY_F17:
        return GK_F17;
    case GLFW_KEY_F18:
        return GK_F18;
    case GLFW_KEY_F19:
        return GK_F19;
    case GLFW_KEY_F20:
        return GK_F20;
    case GLFW_KEY_F21:
        return GK_F21;
    case GLFW_KEY_F22:
        return GK_F22;
    case GLFW_KEY_F23:
        return GK_F23;
    case GLFW_KEY_F24:
        return GK_F24;
    case GLFW_KEY_F25:
        return GK_UNKNOWN;
    case GLFW_KEY_KP_0:
        return GK_NUMPAD0;
    case GLFW_KEY_KP_1:
        return GK_NUMPAD1;
    case GLFW_KEY_KP_2:
        return GK_NUMPAD2;
    case GLFW_KEY_KP_3:
        return GK_NUMPAD3;
    case GLFW_KEY_KP_4:
        return GK_NUMPAD4;
    case GLFW_KEY_KP_5:
        return GK_NUMPAD5;
    case GLFW_KEY_KP_6:
        return GK_NUMPAD6;
    case GLFW_KEY_KP_7:
        return GK_NUMPAD7;
    case GLFW_KEY_KP_8:
        return GK_NUMPAD8;
    case GLFW_KEY_KP_9:
        return GK_NUMPAD9;
    case GLFW_KEY_KP_DECIMAL:
        return GK_DECIMAL;
    case GLFW_KEY_KP_DIVIDE:
        return GK_DIVIDE;
    case GLFW_KEY_KP_MULTIPLY:
        return GK_MULTIPLY;
    case GLFW_KEY_KP_SUBTRACT:
        return GK_SUBTRACT;
    case GLFW_KEY_KP_ADD:
        return GK_ADD;
    case GLFW_KEY_KP_ENTER:
        return GK_RETURN;
    case GLFW_KEY_KP_EQUAL:
        return GK_OEM_PLUS;
    case GLFW_KEY_LEFT_SHIFT:
        return GK_SHIFT;
    case GLFW_KEY_LEFT_CONTROL:
        return GK_CONTROL;
    case GLFW_KEY_LEFT_ALT:
        return GK_MENU;
    case GLFW_KEY_LEFT_SUPER:
        return GK_LWIN;
    case GLFW_KEY_RIGHT_SHIFT:
        return GK_SHIFT;
    case GLFW_KEY_RIGHT_CONTROL:
        return GK_CONTROL;
    case GLFW_KEY_RIGHT_ALT:
        return GK_MENU;
    case GLFW_KEY_RIGHT_SUPER:
        return GK_RWIN;
    case GLFW_KEY_MENU:
        return GK_UNKNOWN;
    default:
        return GK_UNKNOWN;
    }
}