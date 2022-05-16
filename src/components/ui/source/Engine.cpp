#include "pch.h"
#include "Engine.h"
#include "View.h"
#include "Defines.h"
#include "glfw/RenderWindow.h"

namespace UIEngine
{

const ultralight::String assets_path = "./assets/";
const ultralight::String resource_path = "./resources/";

Engine::Engine()
{
    _config.SetMSAAEnabled(false);
    _render_window = std::make_unique<UIEngine::RenderWindow>(true, _config.GetMSAAEnabled());

}

Engine::~Engine()
{
    //UILoader::UnSubscribeLoader();
}

void Engine::Init(GLFWwindow* window)
{
    _gpu_driver = std::make_unique<GPUDriverGL>(window, &_config);

    ultralight::Config config;
    config.face_winding = ULTRALIGHT_FACE_WIDTHING;
    config.resource_path_prefix = resource_path;

    ultralight::Platform::instance().set_config(config);
    ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
    ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
    ultralight::Platform::instance().set_gpu_driver(_gpu_driver.get());
    ultralight::Platform::instance().set_logger(&_logger);

    _render = ultralight::Renderer::Create();
    AddTitleWithUrl("file://welcome.html", 1000, 1000, 1, true);

}

void CreateAndOpenWindow()
{

}

void Engine::AddTitleWithUrl(const std::string& url, int width, int height, double scale, bool is_selected)
{
    _view = std::make_unique<View>(_render, width, height, scale);

    _view->GetView()->LoadURL("file:///assets/welcome.html");
    _view->GetView()->set_view_listener(this);
    _view->GetView()->set_load_listener(this);
    _view->GetView()->Focus();
    //view->GetSurface()->Resize(width, height);


    //m_views.push_back(std::unique_ptr<UIView>(view));

    //if (is_selected)
    //{
    //    SelectLastView();
    //}


}

void Engine::Update()
{
    _render->Update();
}

void Engine::Render()
{
    _render->Render();
}


void Engine::OnChangeCursor(ultralight::View* caller,
                            ultralight::Cursor cursor)
{

}

void Engine::OnUILoaded(ultralight::View* loaded)
{

}

View* Engine::GetView()
{
    return _view.get();
}

RenderWindow* Engine::GetRenderWindow()
{
    return _render_window.get();
}

GPUDriverGL* Engine::GetGPUDriver()
{
    return _gpu_driver.get();
}

void Engine::updateGeometry()
{
    bool initial_creation = false;
    ultralight::RenderTarget target = _view->GetView()->render_target();

    if (_vertices.empty())
    {
        _vertices.resize(4);
        _indices.resize(6);
        int patternCCW[] = { 0, 3, 1, 1, 3, 2 };
        int patternCW[] = { 0, 1, 3, 1, 2, 3 };
        auto& config = ultralight::Platform::instance().config();
        if (config.face_winding == ultralight::FaceWinding::Clockwise)
            memcpy(_indices.data(), patternCW, sizeof(int) * _indices.size());
        else
            memcpy(_indices.data(), patternCCW, sizeof(int) * _indices.size());

        memset(&_gpu_state, 0, sizeof(_gpu_state));
        ultralight::Matrix identity;
        identity.SetIdentity();

        _gpu_state.viewport_width = width;
        _gpu_state.viewport_height = width;
        _gpu_state.transform = identity.GetMatrix4x4();
        _gpu_state.enable_blend = true;
        _gpu_state.enable_texturing = true;
        _gpu_state.shader_type = ultralight::ShaderType::Fill;
        _gpu_state.render_buffer_id = 0;
        _gpu_state.texture_1_id = target.texture_id;

        initial_creation = true;
    }

    if (!_needs_update)
        return;

    ultralight::Vertex_2f_4ub_2f_2f_28f v;
    memset(&v, 0, sizeof(v));

    v.data0[0] = 1; // Fill Type: Image

    v.color[0] = 255;
    v.color[1] = 255;
    v.color[2] = 255;
    v.color[3] = 255;

    float left = 0.f;
    float top = 0.f;
    float right = width;
    float bottom = height;

    // TOP LEFT
    v.pos[0] = v.obj[0] = left;
    v.pos[1] = v.obj[1] = top;
    v.tex[0] = target.uv_coords.left;
    v.tex[1] = target.uv_coords.top;

    _vertices[0] = v;

    // TOP RIGHT
    v.pos[0] = v.obj[0] = right;
    v.pos[1] = v.obj[1] = top;
    v.tex[0] = target.uv_coords.right;
    v.tex[1] = target.uv_coords.top;

    _vertices[1] = v;

    // BOTTOM RIGHT
    v.pos[0] = v.obj[0] = right;
    v.pos[1] = v.obj[1] = bottom;
    v.tex[0] = target.uv_coords.right;
    v.tex[1] = target.uv_coords.bottom;

    _vertices[2] = v;

    // BOTTOM LEFT
    v.pos[0] = v.obj[0] = left;
    v.pos[1] = v.obj[1] = bottom;
    v.tex[0] = target.uv_coords.left;
    v.tex[1] = target.uv_coords.bottom;

    _vertices[3] = v;

    ultralight::VertexBuffer vbuffer;
    vbuffer.format = ultralight::VertexBufferFormat::_2f_4ub_2f_2f_28f;
    vbuffer.size = static_cast<uint32_t>(sizeof(ultralight::Vertex_2f_4ub_2f_2f_28f) * _vertices.size());
    vbuffer.data = (uint8_t*)_vertices.data();

    ultralight::IndexBuffer ibuffer;
    ibuffer.size = static_cast<uint32_t>(sizeof(ultralight::IndexType) * _indices.size());
    ibuffer.data = (uint8_t*)_indices.data();

    if (initial_creation)
    {
        _geometry_id = _gpu_driver->NextGeometryId();
        _gpu_driver->CreateGeometry(_geometry_id, vbuffer, ibuffer);
    }
    else
    {
        _gpu_driver->UpdateGeometry(_geometry_id, vbuffer, ibuffer);
    }

    _needs_update = false;
}

void Engine::Draw()

{
    // m_render->Update();
    // m_render->Render();


    //if (m_gpuDriver->HasCommandsPending())
    //{
    //    m_gpuDriver->DrawCommandList();
    //}
    updateGeometry();

    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_NEVER);
    //
    _gpu_driver->DrawGeometry(_geometry_id, 6, 0, _gpu_state);
    //
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindBuffer(GL_ARRAY_BUFFER, 0);







}

}