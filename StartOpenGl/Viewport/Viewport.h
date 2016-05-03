#pragma once

#include "World\Shapes\IViewportVisitor.h"

namespace Renderer
{
    class Renderer;
}

namespace Viewport
{
    class Camera;

    class CameraState
    {
    public:
        virtual void SetNoneMode() = 0;
        virtual void SetRotateMode(int x, int y) = 0;
        virtual void SetZoomMode(int x, int y) = 0;
        virtual void KeepMouseInWindow(HWND wnd, int x, int y) = 0;
        virtual void UpdateCamera(Camera*) = 0;
    };

    class Viewport final : World::IViewportVisitor
    {
    public:
        Viewport(void* windowHandle);
        ~Viewport();

    public:
        HWND GetWnd() const;

        void BeginScene() const;
        void EndScene() const;

        void OnMouseLDown(int x, int y);
        void OnMouseRDown(int x, int y);
        void OnMouseMove(int x, int y);
        void OnMouseLUp(int x, int y);
        void OnMouseRUp(int x, int y);
        void OnKeyDown(int keyCode);
        void OnKeyUp(int keyCode);
        void OnWheelRotate(int delta);

    public:
        void DrawWorld(const void* worldHandle);
        void DrawHUD() const;

	private:
		void GetViewportDims(uint32 &width, uint32 &height) const;
        void DrawBox(const World::Shape& shape) override;

    private:
        HWND _wnd;

        std::unique_ptr<Camera> _camera;
        std::unique_ptr<CameraState> _cameraState;
        std::unique_ptr<Renderer::Renderer> _renderer;
    };
}
