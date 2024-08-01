// Made by Dominick O'Hara
// April 30, 2024
#include "Sim3DUtils.h"

void Sim3DUtils::DrawCubeWithBoundingBox(Vector3 pos, float width, float height, float length, Color cubeColor, Color boundingColor)
{
	Mesh cube = GenMeshCube(width, height, length);
	Model cubeModel = LoadModelFromMesh(cube);
	BoundingBox bounding = GetModelBoundingBox(cubeModel);
	DrawModel(cubeModel, pos, 1.0f, cubeColor);
	DrawBoundingBox(bounding, boundingColor);
}

void Sim3DUtils::DrawArrow(Vector3 pos, float length, float arrowSize, Color color)
{
	rlPushMatrix();
		rlTranslatef(pos.x, pos.y, pos.z);
		DrawCube({ 0.0f, 0.0f, 0.0f }, 0.1f, 0.1f, length, color);
		Model coneModel = LoadModelFromMesh(GenMeshCone(arrowSize, arrowSize * 2, 6));
		rlTranslatef(0.0f, 0.0f, length / 2);
		rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		DrawModel(coneModel, { 0, 0, 0 }, 1.0f, color);
	rlPopMatrix();
}

void Sim3DUtils::DrawAxis(Vector3 pos, float length, Vector3 axisOffsets)
{
	Vector3 vec0 = { 0.0f, 0.0f, 0.0f };
	// x-axis
	rlPushMatrix();
		rlTranslatef(pos.x, pos.y, pos.z);
			// x axis
		rlPushMatrix();
			rlTranslatef(axisOffsets.x + length / 2, 0.0f, 0.0f);
			rlRotatef(90, 0.0f, 1.0f, 0.0f);
			DrawArrow(vec0, length, 0.25f, { 255, 0, 0, 255 });
		rlPopMatrix();
			// y axis
		rlPushMatrix();
			rlTranslatef(0.0f, axisOffsets.y + length / 2, 0.0f);
			rlRotatef(90, -1.0f, 0.0f, 0.0f);
			DrawArrow(vec0, length, 0.25f, { 0, 255, 0, 255 });
		rlPopMatrix();
			// z axis
		rlPushMatrix();
			rlTranslatef(0.0f, 0.0f, axisOffsets.z + length / 2);
			DrawArrow(vec0, length, 0.25f, { 0, 0, 255, 255 });
		rlPopMatrix();
	rlPopMatrix();
}
