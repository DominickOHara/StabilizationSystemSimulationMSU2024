// Made by Dominick O'Hara
// April 30, 2024
#pragma once

// here because some headers define PI
#ifndef PI
#define PI 3.14159265358979323846f
#endif
/** @brief Structure to store Cardanian angles. I.E. pitch, roll, yaw (XYZ) order */
struct Cardanian3 {
	float pitch; /** @brief Pitch angle (X-Axis) */
	float roll; /** @brief Roll angle (Y-Axis) */
	float yaw; /** @brief Yaw angle (Z-Axis) */

	/** @brief Makes sure the radians are between 0 and 2PI */
	void fixRadians() {
		pitch = (pitch >= PI * 2) ? pitch - PI * 2 : pitch;
		roll = (roll >= PI * 2) ? roll - PI * 2 : roll;
		yaw = (yaw >= PI * 2) ? yaw - PI * 2 : yaw;
		pitch = (pitch <= 0.0f) ? PI * 2 - pitch : pitch;
		roll = (roll <= PI * 2) ? PI * 2 - roll : roll;
		yaw = (yaw <= PI * 2) ? PI * 2 - yaw : yaw;
	}
};