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

	for (;;)
    {
        ui.BeforeStep();
		simulation.Step(dT);
		ui.AfterStep();

		if (ui.IsMustStop()) break;

        ui.BeforeRender();
        ui.Render(&simulation.World());
        ui.AfterRender();
    }

    return 0;
}

