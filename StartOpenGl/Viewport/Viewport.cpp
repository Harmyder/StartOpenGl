#include "stdafx.h"

#include "Viewport.h"

#include "Renderer\Renderer.h"
#include "Viewport\Camera.h"
#include "World\World.h"
#include "World\Shapes\Box.h"
#include "Utilities\Print\DebugPrint.h"

using namespace std;

namespace
{
    glm::vec4 BOX_COLOR{ .5f, .7f, 0.f, 1.f };
    glm::vec4 X_COLOR{ 1.f, 0.f, 0.f, 1.f };
    glm::vec4 Y_COLOR{ 0.f, 1.f, 0.f, 1.f };
    glm::vec4 Z_COLOR{ 0.f, 0.f, 1.f, 1.f };
}

namespace Viewport
{
    class CameraStateDefault final : public CameraState
    {
    public:
        void SetNoneMode() override {}
        void SetRotateMode(int, int) override {}
        void SetZoomMode(int, int) override {}
        void KeepMouseInWindow(HWND, int, int) override {}
        void UpdateCamera(Camera*) override {}
    };

    class CameraStateActive final : public CameraState
    {
        enum class Mode { none, rotate, zoom };

    public:
        void SetNoneMode() override { _mode = Mode::none; }
        void SetRotateMode(int x, int y) override { SetMode(Mode::rotate, x, y); }
        void SetZoomMode(int x, int y) override { SetMode(Mode::zoom, x, y); }

        void KeepMouseInWindow(HWND wnd, int x, int y) override
        {
            if (_mode != Mode::none)
            {
                RECT rect;
                GetClientRect(wnd, &rect);
                const int width = rect.right - rect.left;
                const int height = rect.bottom - rect.top;

                POINT p = { x, y };
                if (x < 0) p.x = width;
                if (x > width) p.x = 0;

                if (y < 0) p.y = height;
                if (y > height) p.y = 0;

                if (p.x != x || p.y != y)
                {
                    _delta = POINT{ 0, 0 };
                    POINT screen = p;
                    ClientToScreen(wnd, &screen);
                    SetCursorPos(screen.x, screen.y);
                }
                else
                {
                    _delta = POINT{ p.x - _currentMousePosition.x, p.y - _currentMousePosition.y };
                }
                _currentMousePosition = p;
            }
        }
       
        void UpdateCamera(Camera* camera) override 
        {
            switch (_mode)
            {
            case Mode::rotate:
            {
                RotateCamera(camera);
            }
                break;
            case Mode::zoom:
                ZoomCamera(camera);
                break;
            default:
                break;
            }
        }

    private:
        void SetMode(Mode mode, int x, int y)
        {
            _mode = mode;
            _currentMousePosition = POINT{ x, y };
        }

        void RotateCamera(Camera* camera)
        {
            camera;
        }

        void ZoomCamera(Camera *camera)
        {
            camera->MoveForward(_delta.y / 10.f);
        }

    private:
        Mode _mode = Mode::none;
        POINT _delta;
        POINT _currentMousePosition;
    };

    Viewport::Viewport(void* windowHandle) :
        _wnd(static_cast<HWND>(windowHandle)),
        _renderer(make_unique<Renderer::Renderer>()),
        _camera(make_unique<Camera>()),
        _cameraState(make_unique<CameraStateDefault>())
    {
        _renderer->Initialize(_wnd);
    }

    Viewport::~Viewport() {}

    HWND Viewport::GetWnd() const
    {
        return _wnd;
    }

    void Viewport::BeginScene() const
    {
        _renderer->SetCameraTransform(_camera->GetTransform());

        uint32 width;
		uint32 height;
		GetViewportDims(width, height);
		_renderer->BeginScene(width, height);
    }

    void Viewport::EndScene() const
    {
		_renderer->EndScene();
    }

    void Viewport::OnMouseLDown(int x, int y)
    {
        _cameraState->SetRotateMode(x, y);
    }

    void Viewport::OnMouseLUp(int x, int y)
    {
        UNREFERENCED_PARAMETER(x);
        UNREFERENCED_PARAMETER(y);
        _cameraState->SetNoneMode();
    }

    void Viewport::OnMouseRDown(int x, int y)
    {
        _cameraState->SetZoomMode(x, y);
    }

    void Viewport::OnMouseRUp(int x, int y)
    {
        UNREFERENCED_PARAMETER(x);
        UNREFERENCED_PARAMETER(y);
        _cameraState->SetNoneMode();
    }

    void Viewport::OnMouseMove(int x, int y)
    {
        _cameraState->KeepMouseInWindow(_wnd, x, y);
        _cameraState->UpdateCamera(_camera.get());
    }

    void Viewport::OnKeyDown(int keyCode)
    {
        switch (keyCode)
        {
        case VK_MENU:
            _cameraState = make_unique<CameraStateActive>();
            SetCapture(_wnd);
            break;
        }
    }

    void Viewport::OnKeyUp(int keyCode)
    {
        switch (keyCode)
        {
        case VK_MENU:
            _cameraState = make_unique<CameraStateDefault>();
            ReleaseCapture();
            break;
        }
    }

    void Viewport::OnWheelRotate(int delta)
    {
        _camera->MoveForward(delta / 50.f);
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
		GetClientRect(_wnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

    void Viewport::DrawHUD() const
    {
        _renderer->DisableDepth();
        _renderer->DrawArrowShape(glm::vec3(0.f, 1.f, -80.f), glm::vec3(0.f, 1.f, 0.f), 5.f, 4.f, BOX_COLOR);
        _renderer->EnableDepth();
    }
}
