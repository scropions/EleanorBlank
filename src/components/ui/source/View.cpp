#include "pch.h"
#include "View.h"
#include "Logger.h"

namespace UIEngine
{

View::View(ultralight::RefPtr<ultralight::Renderer> renderer, int width, int height, double scale)
{
    ultralight::ViewConfig viewConfig;
    viewConfig.initial_device_scale = scale;
    viewConfig.is_accelerated = true;
    viewConfig.is_transparent = true;
    _view = renderer->CreateView(width, height, viewConfig, nullptr);
}

View::View(ultralight::RefPtr<ultralight::View> existing_view) : _view(existing_view)
{
}

void View::ToggleTransparency()
{
    _view->EvaluateScript("document.body.style.background = 'transparent'");
}

ultralight::RefPtr<ultralight::View> View::GetView()
{
    return _view;
}

ITextureSurface* View::GetSurface()
{
    return static_cast<ITextureSurface*>(_view->surface());
}
}