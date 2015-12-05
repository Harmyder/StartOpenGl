// StartOpenGl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "UserInteractor.h"
#include "Simulations\CubeSimulation.h"

using namespace std;

int _tmain(int, _TCHAR*)
{
    const float dT = 1.f / 60;
    UserInteractor ui(GetModuleHandle(NULL));
    ui.SetDeltaTime(dT);

	Simulations::CubeSimulation simulation;

    for (int i = 0; !ui.IsMustStop(); ++i)
    {
        ui.BeforeStep();
		simulation.Step(dT);
		ui.AfterStep();

        ui.BeforeRender();
        ui.Render(&simulation.World());
        ui.AfterRender();
    }

    return 0;
}

