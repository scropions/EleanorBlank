#pragma once
#include "Logger.h"
#include "Loader.h"
#include "Config.h"
#include "glfw/GPUDriverGL.h"
#include "glfw/RenderWindow.h"

typedef struct GLFWwindow GLFWwindow;

namespace UIEngine
{
class View;
class Engine: public ultralight::ViewListener, public Loader
{
public:
    Engine();
    ~Engine();

    void Init(GLFWwindow* window);

    void AddTitleWithUrl(const std::string& url, int width, int height, double scale, bool is_selected = false);
    void Update();
    void Render();

    RenderWindow* GetRenderWindow();
    GPUDriverGL* GetGPUDriver();


    void OnUILoaded(ultralight::View* loaded);

    void OnChangeCursor(ultralight::View* caller,
                        ultralight::Cursor cursor) override;

    void Draw();

    void DrawGeometry()
    {
        _gpu_driver->DrawGeometry(_geometry_id, 6, 0, _gpu_state);
    }

    void updateGeometry();

    View* GetView();

private:
    ultralight::GPUState _gpu_state;
    ultralight::RefPtr<ultralight::Renderer> _render;

    std::unique_ptr<GPUDriverGL> _gpu_driver;
    std::unique_ptr<RenderWindow> _render_window;
    std::unique_ptr<View> _view;

    Config _config;
    Logger _logger;

    uint32_t _geometry_id;
    std::vector<ultralight::Vertex_2f_4ub_2f_2f_28f> _vertices;
    std::vector<ultralight::IndexType> _indices;


    int width = 1000;
    int height = 1000;
    bool _needs_update = true;
};
}