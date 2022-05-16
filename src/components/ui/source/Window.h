typedef struct GLFWwindow GLFWwindow;
typedef struct GLFWcursor GLFWcursor;

namespace UIEngine
{

class Engine;

class UIWindow
{
public:
    UIWindow(uint32_t width, uint32_t height,
             bool fullscreen, unsigned int window_flags);

    virtual ~UIWindow();

    virtual uint32_t screen_width() const;

    virtual uint32_t width() const;

    virtual uint32_t screen_height() const;

    virtual uint32_t height() const;

    virtual void MoveTo(int x, int y);

    virtual void MoveToCenter();

    virtual int x() const;

    virtual int y() const;

    virtual uint32_t GetRenderBufferID() const { return 0; }

    virtual bool IsAccelerated() const { return true; }

    virtual bool IsFullscreen() const { return _fullscreen; }

    virtual double scale() const;

    virtual void SetTitle(const char* title);

    virtual void SetCursor(ultralight::Cursor cursor);

    virtual void Show();

    virtual void Hide();

    virtual bool Visible() const;

    virtual void Close();

    virtual int ScreenToPixels(int val) const
    {
        return (int)round(val * scale());
    }

    virtual int PixelsToScreen(int val) const
    {
        return (int)round(val / scale());
    }

    virtual void* native_handle() const;

    void Repaint();

    void InvalidateWindow() { _need_repaint = true; }

    virtual void OnKeyEvent(const ultralight::KeyEvent& evt) ;
    virtual void OnMouseEvent(const ultralight::MouseEvent& evt) ;
    virtual void OnScrollEvent(const ultralight::ScrollEvent& evt) ;
    virtual void OnResize(int width, int height);


    void run();
protected:
    std::unique_ptr<Engine> _engine;

    GLFWcursor* _cursor_ibeam;
    GLFWcursor* _cursor_crosshair;
    GLFWcursor* _cursor_hand;
    GLFWcursor* _cursor_hresize;
    GLFWcursor* _cursor_vresize;

    GLFWwindow* _window = nullptr;

    bool _fullscreen = false;
    bool _need_repaint = false;
};
}