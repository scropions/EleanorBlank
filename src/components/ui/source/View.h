#include "interfaces/ui/Interfaces.h"

namespace UIEngine
{
class View
{
public:
    View() = delete;
    View(ultralight::RefPtr<ultralight::Renderer> renderer, int width, int height, double scale);
    View(ultralight::RefPtr<ultralight::View> existing_view);

    ultralight::RefPtr<ultralight::View> GetView();

    void ToggleTransparency();

    ITextureSurface* GetSurface();
private:
    ultralight::RefPtr<ultralight::View> _view;
    bool _is_transparent_ = false;
};
}