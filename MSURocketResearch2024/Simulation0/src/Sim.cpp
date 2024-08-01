// Made by Dominick O'Hara
// April 30, 2024
#include "Sim.h"
#include "Sim3DUtils.h"

#include <iostream>
const float Sim::DEFAULT_CANARD_RPS = 24.0f * DEG2RAD;

Sim::Sim() {

	settings[USE_DEGREES] = true;
	settings[DISPLAY_ROCKET_AXIS] = true;
	settings[DISPLAY_WORLD_AXIS] = true;
	settings[INSTANT_FIN_ROT] = false;
	settings[USE_SERIAL] = false;
	settings[USE_STABL_SYS] = false;

	// setup camera
	float camera_speed = 5.0f;
	cam.position = { -10.0f, 10.0f, 0.0f };
	cam.projection = CAMERA_PERSPECTIVE;
	cam.target = { 0.0f, 0.0f, 0.0f };
	cam.up = { 0.0f, 1.0f, 0.0f };
	cam.fovy = 90;

	rocketInstance = new Rocket();
	serialInstance = new ArduinoSerial();
}

Sim::~Sim() {
	delete serialInstance;
	delete rocketInstance;
}

void Sim::Update(float deltaTime) {
	// Update camera
	if (IsKeyDown(KEY_W)) {
		cam.position.x += camera_speed * deltaTime;
		cam.target.x += camera_speed * deltaTime;
	}
	if (IsKeyDown(KEY_S)) {
		cam.position.x -= camera_speed * deltaTime;
		cam.target.x -= camera_speed * deltaTime;
	}
	if (IsKeyDown(KEY_A)) {
		cam.position.z -= camera_speed * deltaTime;
		cam.target.z -= camera_speed * deltaTime;
	}
	if (IsKeyDown(KEY_D)) {
		cam.position.z += camera_speed * deltaTime;
		cam.target.z += camera_speed * deltaTime;
	}

	// update rocket
	rocketInstance->SetOrientation(rocket_orientation); // set last orientation
	rocketInstance->SetAllGoalCanardRotation(canard_rotations);
	
	// get orientation input and canard rotation input
	Cardanian3 fetched_orientation = rocketInstance->GetOrientation();
	rocketInstance->UpdateCanardFins(deltaTime);

	if (SerialEnabled()) {
		serialInstance->Update(deltaTime);
		fetched_orientation = serialInstance->GetOrientation();
	}

	if (GetSetting(USE_STABL_SYS)) {
		// placeholder stablization system if serial unavilable
		if (!SerialEnabled()) {
			for (int i = 0; i < 4; i++) {
				canard_rotations[i] += 90.0f * DEG2RAD * deltaTime;
				if (canard_rotations[i] >= 2 * PI) {
					canard_rotations[i] = canard_rotations[i] - 2 * PI;
				}
			}
		}
		// get canard rotations from serial input
		else {
			serialInstance->GetCanardRotation(canard_rotations);
		}
	}
	rocket_orientation.roll = fetched_orientation.roll;
	rocket_orientation.pitch = fetched_orientation.pitch;
	rocket_orientation.yaw = fetched_orientation.yaw;

	if (GetSetting(INSTANT_FIN_ROT))
		rocketInstance->SetCanardRPS(99999.0f);
	else 
		rocketInstance->SetCanardRPS(canardRPS);
}

void Sim::Draw(float deltaTime)
{
	// draw 3d contents
	BeginMode3D(cam);
	DrawGrid(10, 1.0f);
		if (GetSetting(DISPLAY_WORLD_AXIS)) {
			Sim3DUtils::DrawAxis({ 0.0f, 0.0f, 0.0f }, 100.0f, { 0.0f, 0.0f, 0.0f }); 
		}
		if (GetSetting(DISPLAY_ROCKET_AXIS)) {
			rocketInstance->DrawAxis();
		}
		rocketInstance->Draw(deltaTime);
	EndMode3D();
}