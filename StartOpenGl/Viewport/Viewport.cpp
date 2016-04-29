#include "stdafx.h"

#include "Viewport.h"

#include "Renderer\Renderer.h"
#include "Viewport\Camera.h"
#include "World\World.h"
#include "World\Shapes\Box.h"

namespace
{
	glm::vec4 BOX_COLOR{ 0.5, 0.7, 0.0, 1.0 };
}

namespace Viewport
{
    Viewport::Viewport(void* windowHandle) :
        _Wnd(static_cast<HWND>(windowHandle)),
        _renderer(new Renderer::Renderer),
        _camera(new Camera)
    {
        _renderer->Initialize(_Wnd);
    }

    Viewport::~Viewport() {}

    HWND Viewport::GetWnd() const
    {
        return _Wnd;
    }

    void Viewport::BeginScene() const
    {
		uint32 width;
		uint32 height;
		GetViewportDims(width, height);
		_renderer->BeginScene(width, height);
    }

    void Viewport::EndScene() const
    {
		_renderer->EndScene();
    }

    void Viewport::OnKeyDown(int keyCode)
    {
        UNREFERENCED_PARAMETER(keyCode);
    }

    void Viewport::OnKeyUp(int keyCode)
    {
        UNREFERENCED_PARAMETER(keyCode);
    }

    void Viewport::OnWheelRotate(int delta)
    {
        UNREFERENCED_PARAMETER(delta);
    }

    void Viewport::DrawWorld(const void* worldHandle)
    {
		const World::World& world = *reinterpret_cast<const World::World*>(worldHandle);

        for (auto const& pe : world.GetShapes())
        {
            pe->Accept(*this);
        }
    }

    void Viewport::DrawBox(const World::Shape& shape)
    {
        const World::Box& box = static_cast<const World::Box&>(shape);
        _renderer->DrawBoxShape(box.GetTransform(), box.GetHalfExtents(), BOX_COLOR);
    }

	void Viewport::GetViewportDims(uint32 &width, uint32 &height) const
	{
		RECT rect;
		GetClientRect(_Wnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

    void Viewport::DrawHUD() const
    {
        _renderer->DrawArrowShape(glm::vec3(0.f, 1.f, -80.f), glm::vec3(0.f, 1.f, 0.f), 5.f, 4.f, BOX_COLOR);
    }
}
