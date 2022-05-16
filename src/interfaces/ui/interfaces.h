#pragma once

namespace UIEngine
{
	class ITextureSurface : public ultralight::Surface {
	public:
		virtual ~ITextureSurface() {}

		virtual unsigned int GetTextureAndSyncIfNeeded() = 0;
	};

    class IWindowListener
    {
    public:
        virtual ~IWindowListener() {}

        virtual void OnClose() = 0;
        virtual void OnResize(uint32_t width, uint32_t height) = 0;
        virtual void OnChangeFocus(bool focused) = 0;
        virtual void OnKeyEvent(const ultralight::KeyEvent& evt) = 0;
        virtual void OnMouseEvent(const ultralight::MouseEvent& evt) = 0;
        virtual void OnScrollEvent(const ultralight::ScrollEvent& evt) = 0;
    };
}