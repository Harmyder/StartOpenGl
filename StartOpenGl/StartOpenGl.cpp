// StartOpenGl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "UserInteractor.h"

using namespace std;

int _tmain(int, _TCHAR*)
{
    const float dT = 1.f / 60;
    UserInteractor ui(GetModuleHandle(NULL));
    ui.SetDeltaTime(dT);

    for (int i = 0; !ui.IsMustStop(); ++i)
    {
        ui.BeforeStep();
        ui.AfterStep();

        ui.BeforeRender();
        ui.Render();
        ui.AfterRender();
    }

    return 0;
}

