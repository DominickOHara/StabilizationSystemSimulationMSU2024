// Made by Dominick O'Hara
// April 30, 2024
#include "Rocket.h"
#include "rlgl.h"
#include "raymath.h"
#include "Sim3DUtils.h"


const float Rocket::DEFAULT_CANARD_RPS = 24.0f * DEG2RAD;

void Rocket::ApplyCoordsToMatrix() {
	rlTranslatef(position.x, position.y, position.z);
	Matrix rot = MatrixRotateXYZ({ 
		orientation.pitch, 
		orientation.roll,
		orientation.yaw
	});
	rlMultMatrixf((const float*)&rot);
	//rlRotatef(orientation.pitch * RAD2DEG, 1.0f, 0.0f, 0.0f);
	//rlRotatef(orientation.roll * RAD2DEG, 0.0f, 1.0f, 0.0f);
	//rlRotatef(orientation.yaw * RAD2DEG, 0.0f, 0.0f, 1.0f);
}

void Rocket::DrawCanard(int bodyFace, float bodyHeight, float canardRotation)
{
	rlPushMatrix();
		rlTranslatef(0.0f, bodyHeight - ROCKET_HEIGHT / 2, 0.0f);
		rlRotatef(bodyFace * 90.0f, 0.0f, 1.0f, 0.0f);
		rlTranslatef(0.0f, 0.0f, 0.75f);
		rlRotatef(canardRotation * RAD2DEG, 0.0f, 0.0f, 1.0f);
		Sim3DUtils::DrawCubeWithBoundingBox(
			position,
			0.1f, 1.0f, 0.5f,
			{ 125, 125, 125, 255 },
			{ 255, 255, 255, 255 }
		);
	rlPopMatrix();
}


void Rocket::UpdateCanardFins(float deltaTime)
{
	if (canardRPS > 90000.0f) {
		for (int i = 0; i < 4; i++)
			canard_rotations[i] = goal_canard_rotations[i];
		return;
	}
	
	float radDelta = canardRPS * deltaTime;
	for (int i = 0; i < 4; i++)
	{
		// rotate clockwise or counter clockwise based on which is closer
		bool dir = signbit(goal_canard_rotations[i] - canard_rotations[i]);
		if (dir == true) { // negative
			canard_rotations[i] -= radDelta;
			if (canard_rotations[i] < goal_canard_rotations[i])
				canard_rotations[i] = goal_canard_rotations[i];
		}
		else { // positive
			canard_rotations[i] += radDelta;
			if (canard_rotations[i] > goal_canard_rotations[i])
				canard_rotations[i] = goal_canard_rotations[i];
		}
	}
}

void Rocket::DrawAxis() {
	rlPushMatrix();
		ApplyCoordsToMatrix();
		Sim3DUtils::DrawAxis(position, 3.0f, { 2.0f, 6.0f, 2.0f });
	rlPopMatrix();
}

void Rocket::Draw(float deltaTime)
{
	rlPushMatrix();
		ApplyCoordsToMatrix();
		Sim3DUtils::DrawCubeWithBoundingBox(
			position,
			1.0f, ROCKET_HEIGHT, 1.0f,
			{ 125, 125, 125, 255 },
			{ 255, 255, 255, 255 }
		);
		for (int i = 0; i < 4; i++) {
			DrawCanard(i, 9.0f, GetCanardRotation(i));
		}
	rlPopMatrix();
}