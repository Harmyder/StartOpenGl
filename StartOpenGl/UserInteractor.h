#pragma once

#include "SDK\Timer.h"

namespace Viewport
{
    class Viewport;
}

class UserInteractor final
{
    static UserInteractor *s_Instance;

    UserInteractor(UserInteractor const&) = delete;
    UserInteractor & operator=(const UserInteractor &) = delete;

public:
    static UserInteractor &GetInstance();
    UserInteractor(HINSTANCE instance);
    ~UserInteractor();

    Viewport::Viewport& GetViewport();

    void SetDeltaTime(float dT);
    float GetDeltaTime();

    void BeforeStep();
    void AfterStep();

    void BeforeRender();
    void Render(const void* world);
    void AfterRender();

    bool IsMustStop();

private:
    bool TreatMessages();

    void WaitForDeltaTime();

    void OnLostFocus();
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);
    void OnWheelRotate(int delta);

private:
    enum { INITIAL_WINDOW_WIDTH = 800, INITIAL_WINDOW_HEIGHT = 600 };
    HWND CreateDemoWindow(HINSTANCE instance);

    static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    bool _mustStop;

    std::unique_ptr<Viewport::Viewport> _viewport;

    SDK::Timer _Timer;
    float _DeltaTime;
    unsigned __int64 _StartTicks;
};
