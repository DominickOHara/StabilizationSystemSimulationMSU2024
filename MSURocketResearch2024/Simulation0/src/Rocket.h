// Made by Dominick O'Hara
// April 30, 2024
#pragma once

#include "raylib.h"
#include "Cardanian3.h"

class Rocket {
public:


	/** @brief Default Radians Per Second */
	static const float DEFAULT_CANARD_RPS;

	/** @brief Renders the rocket onto the screen */
	void Draw(float deltaTime);

	/** @brief  Draws Visualization of 3D Rocket Axis */
	void DrawAxis();

	inline Cardanian3 GetOrientation() { return this->orientation; }
	
	inline void SetOrientation(Cardanian3& orientation) {this->orientation = orientation;}
	
	/** @param rpm Radians Per Second */
	inline void SetCanardRPS(float rps) { canardRPS = rps; }
	
	/**
	 * @param canardIndex specific canard fin. Valid values are 0 - 3.
	 * @return goal rotation of the specified canard fin (Radians)
	 *	 this dose not return the current rotation, but the goal rotation
	 */
	inline float GetGoalCanardRotation(int canardIndex) { return this->goal_canard_rotations[canardIndex]; }
	
	/**
	 * @param canardIndex specific canard fin. Valid values are 0 - 3.
	 * @return rotation of the specified canard fin (Radians)
	 *	returns the actual rotation of the canard fins
	 */
	inline float GetCanardRotation(int canardIndex) { return this->canard_rotations[canardIndex]; }

	/**
	 * @brief Copies the array of goal canard rotations (Radians) into the inputted array.
	 * 		this dose not return the current rotation, but the goal rotation.
	 * @param canard_rotations a pointer to a float array with 4 elements
	 */		 
	inline void GetAllGoalCanardRotation(float goal_canard_rotations[4]) {
		canard_rotations[0] = this->goal_canard_rotations[0];
		canard_rotations[1] = this->goal_canard_rotations[1];
		canard_rotations[2] = this->goal_canard_rotations[2];
		canard_rotations[3] = this->goal_canard_rotations[3];
	}

	/** 
	 * @brief Copies the inputted array of floats into the goal canard rotations array.
	 *		this dose not return the current rotation, but the goal rotation.
	 * @param canard_rotations a pointer to a float array with 4 elements 
	 */
	inline void SetAllGoalCanardRotation(float goal_canard_rotations[4]) {
		this->goal_canard_rotations[0] = goal_canard_rotations[0];
		this->goal_canard_rotations[1] = goal_canard_rotations[1];
		this->goal_canard_rotations[2] = goal_canard_rotations[2];
		this->goal_canard_rotations[3] = goal_canard_rotations[3];
	}

	/**
	 * @brief Copies the array of canard rotations (Radians) into the inputted array.
	 * 		this returns the actual canard rotations
	 * @param canard_rotations a pointer to a float array with 4 elements
	 */
	inline void GetAllCanardRotation(float canard_rotations[4]) {
		canard_rotations[0] = this->canard_rotations[0];
		canard_rotations[1] = this->canard_rotations[1];
		canard_rotations[2] = this->canard_rotations[2];
		canard_rotations[3] = this->canard_rotations[3];
	}

	/**
	 * @brief Copies the inputted array of floats into the canard rotations array.
	 *		this returns the actual canard rotations
	 * @param canard_rotations a pointer to a float array with 4 elements
	 */
	inline void SetAllCanardRotation(float canard_rotations[4]) {
		this->canard_rotations[0] = canard_rotations[0];
		this->canard_rotations[1] = canard_rotations[1];
		this->canard_rotations[2] = canard_rotations[2];
		this->canard_rotations[3] = canard_rotations[3];
	}

	/** @brief Rotates the canard fins to the goal rotations based on their speed */
	void UpdateCanardFins(float deltaTime);

private:

	/** @brief Height of rocket visualization */
	static const int ROCKET_HEIGHT = 10.0f;

	/** @brief how fast canards rotate (Radians per Second) */
	float canardRPS = DEFAULT_CANARD_RPS;
	
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	
	Cardanian3 orientation = { 0.0f, 0.0f, 0.0f };

	/** @brief array of 4 floats; stores the canard fin rotations */
	float canard_rotations[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; 
	
	/** @brief array of 4 floats; stores the rotations the canard fins rotate to */
	float goal_canard_rotations[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	/**
		@brief Multiplies the top matrix on the matrix stack by the rocket's position matrix
		Then by the rocket's orientation matrix.
	*/
	void ApplyCoordsToMatrix();

	/**
		@brief Draws a canard fin given the face and height offset to the rocket. Also applies
		rotation to the canard fin

		@param bodyFace which face to draw the canard fin to. Valid values are 0 - 3 where 0 is the first face
		@param bodyHeight the height offset of the fin relative to the bottom of the rocket body
		@param canardRotation the rotation of the canard fin (Radians)
	*/
	void DrawCanard(int bodyFace, float bodyHeight, float canardRotation);
};