#include "stdafx.h"

#include "Viewport.h"

#include "Renderer\Renderer.h"
#include "Viewport\Camera.h"
#include "World\World.h"
#include "World\Shapes\Box.h"

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
    }

    void Viewport::EndScene() const
    {
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

    void Viewport::DrawWorld(const World::World& world)
    {
        for (auto const& pe : world.GetShapes())
        {
            pe->Accept(*this);
        }
    }

    void Viewport::DrawBox(const World::Shape& shape)
    {
        const World::Box& box = static_cast<const World::Box&>(shape);
        glm::vec4 anyColorHere{ 0.5f, 0.5f, 0.5f, 1.0f };
        _renderer->DrawBoxShape(box.GetTransform(), box.GetHalfExtents(), anyColorHere);
    }
}
