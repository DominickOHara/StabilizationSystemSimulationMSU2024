// Made by Dominick O'Hara
// April 30, 2024
// libraries used:
// STL loader for raylib: https://github.com/WEREMSOFT/stl-loader-for-raylib
// Serial library: https://github.com/wjwwood/serial
// Serial tutorial: https://www.delftstack.com/howto/cpp/cpp-serial-communication/

// the following is defined so that C functions like scanf can be used
// with out Visual Studio throwing 'unsafe' errors
// THIS DEFINITION OF _CRT_SECURE_NO_WARNINGS DOSE NOTHING
// SO I ADDED IT TO THE PRECOMPILED HEADERS UNDER C/C++ DIR INSIDE
// THE PROJECT PROPERTIES (MAKE SURE YOU ARE UNDER ALL RELEASES AND ALL PLATFORMS)!!
#define _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Sim.h"
#include "SimGui.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 840;

int main(int argc, char* argv[])
{
	// init window
	SetTargetFPS(60);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MSU Rocket Research Simulation 2024");
	//Image windowIcon = LoadImage("Assets/WindowIcon.ico");
	Texture2D backgroundTxt = LoadTexture("Assets/Background.png");
	//SetWindowIcon(windowIcon);

	// setup sim
	Sim simInstance;
	SimGui simGuiInstance(&simInstance);
	
	// main loop
	float deltaTime = 0.0f;
	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();
		simInstance.Update(deltaTime);
		BeginDrawing();
			DrawTexture(backgroundTxt, 0, 0, { 255, 255, 255, 255 });
			ClearBackground({0, 0, 25, 255});
			simInstance.Draw(deltaTime);
			simGuiInstance.UpdateAndDraw(deltaTime);
		EndDrawing();
	}

	// end
	CloseWindow();
	return 0;
}