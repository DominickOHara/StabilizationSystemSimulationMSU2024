// Made by Dominick O'Hara
// April 30, 2024
#pragma once
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

namespace Sim3DUtils {
	void DrawCubeWithBoundingBox(Vector3 pos, float width, float height, float length, Color cubeColor, Color boundingColor);
	void DrawArrow(Vector3 pos, float length, float arrowSize, Color color);
	void DrawAxis(Vector3 pos, float length, Vector3 axisOffsets);
}