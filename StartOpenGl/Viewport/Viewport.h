#pragma once

#include "Renderer\Renderer.h"
#include "World\Shapes\IViewportVisitor.h"

namespace Renderer
{
    class Renderer;
}

namespace Viewport
{
    class Camera;

    class Viewport final : World::IViewportVisitor
    {
    public:
        Viewport(void* windowHandle);
        ~Viewport();

    public:
        HWND GetWnd() const;

        void BeginScene() const;
        void EndScene() const;

        void OnKeyDown(int keyCode);
        void OnKeyUp(int keyCode);
        void OnWheelRotate(int delta);

    public:
        void DrawWorld(const void* worldHandle);

	private:
		void GetViewportDims(uint32 &width, uint32 &height) const;
        void DrawBox(const World::Shape& shape) override;

    private:
        HWND      _Wnd;

        std::unique_ptr<Camera> _camera;
        std::unique_ptr<Renderer::Renderer> _renderer;
    };
}
