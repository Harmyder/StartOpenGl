#include "stdafx.h"

#include "UserInteractor.h"
#include "Viewport\Viewport.h"

UserInteractor* UserInteractor::s_Instance = NULL;

UserInteractor& UserInteractor::GetInstance()
{
    return *s_Instance;
}

UserInteractor::UserInteractor(HINSTANCE instance) :
    _viewport(new Viewport::Viewport(CreateDemoWindow(instance))),
    _mustStop(false)
{
    assert(s_Instance == NULL);
    ShowWindow(_viewport->GetWnd(), SW_SHOW);

    s_Instance = this;
}

UserInteractor::~UserInteractor()
{
}

HWND UserInteractor::CreateDemoWindow(HINSTANCE instance)
{
    RECT windowRect = { 0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT };
    char *szWindowClass = "Window class";

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = UserInteractor::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    const int width = windowRect.right - windowRect.left;
    const int height = windowRect.bottom - windowRect.top;
    HWND wnd = CreateWindow(szWindowClass, "Demo Window", WS_OVERLAPPEDWINDOW,
        -width, 0, width, height, NULL, NULL, instance, NULL);

    return wnd;
}

LRESULT CALLBACK UserInteractor::WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    UserInteractor &userInteractor = UserInteractor::GetInstance();
    switch (msg)
    {
    case WM_MOUSEWHEEL:
        userInteractor.OnWheelRotate(GET_WHEEL_DELTA_WPARAM(wParam));
        return 0;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        if (lParam & (1 << 30))
        {
            // this is a repeat
        }
        else
        {
            // first press
            userInteractor.OnKeyDown((int)wParam);
        }
        return 0;
    case WM_SYSKEYUP:
    case WM_KEYUP:
        userInteractor.OnKeyUp((int)wParam);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(wnd, msg, wParam, lParam);
}

void UserInteractor::OnKeyDown(int keyCode)
{
    _viewport->OnKeyDown(keyCode);
}

void UserInteractor::OnKeyUp(int keyCode)
{
    _viewport->OnKeyUp(keyCode);
}

void UserInteractor::OnWheelRotate(int delta)
{
    _viewport->OnWheelRotate(delta);
}

Viewport::Viewport& UserInteractor::GetViewport()
{
    return *_viewport;
}

bool UserInteractor::TreatMessages()
{
    MSG  msg;

    while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    if (!IsWindow(_viewport->GetWnd()))
    {
        return false;
    }

    return true;
}

bool UserInteractor::IsMustStop()
{
    return _mustStop;
}

void UserInteractor::SetDeltaTime(float dT)
{
    _DeltaTime = dT;
}

float UserInteractor::GetDeltaTime()
{
    return _DeltaTime;
}

void UserInteractor::BeforeStep()
{
    _StartTicks = _Timer.GetCounts();
}

void UserInteractor::BeforeRender()
{
    _viewport->BeginScene();
}

void UserInteractor::Render()
{
    _viewport->DrawWorld();
}

void UserInteractor::AfterRender()
{
    _viewport->EndScene();
}

void UserInteractor::AfterStep()
{
    WaitForDeltaTime();
    _mustStop = !TreatMessages();
}

void UserInteractor::WaitForDeltaTime()
{
    unsigned __int64 dtInTicks = static_cast<unsigned __int64>(_DeltaTime * _Timer.GetFrequency());
    unsigned __int64 currentDtInTicks = 0;
    do
    {
        const unsigned __int64 currentTicks = _Timer.GetCounts();
        currentDtInTicks = currentTicks - _StartTicks;
    } while (currentDtInTicks < dtInTicks);
}

