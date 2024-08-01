// Made by Dominick O'Hara
// April 30, 2024
#pragma once

#include "Rocket.h"
#include "ArduinoSerial.h"

class Sim
{

public:
	friend class SimGui;

	/** @brief Enum which represents the various settings */
	static enum SIM_SETTING {
		USE_DEGREES = 0, // Use degrees if on, Use Radians if off
		DISPLAY_ROCKET_AXIS, // Displays axis of rocket
		DISPLAY_WORLD_AXIS, // displays world axis
		INSTANT_FIN_ROT, // makes canard fins snap to goal rotation
		/*
		makes simulation read serial data;
		stablization system will run on external device if enabled and
		the canard fin rotations will be sent over serial
		*/
		USE_SERIAL,
		USE_STABL_SYS, // makes simulation use stablization system
		SETTINGS_COUNT // will always be the amnt of members in enum
	};


	//static const int SETTINGS_COUNT = SIM_SETTING::SETTINGS_COUNT; // number of variables above
	static const float DEFAULT_CANARD_RPS;
	
	// constructor
	Sim();

	// destructor
	~Sim();
	
	/**
	 * @brief Sets a simulation setting
	 * @param setting The setting to set the value
	 * @param val The new value of the setting
	 */
	inline void SetSetting(SIM_SETTING setting, bool val) {
		settings[setting] = val;
	}

	/**
	 * @param setting The specified setting
	 * @return value of the specified setting
	 */
	inline bool GetSetting(SIM_SETTING setting) {
		return settings[setting];
	}

	/**
	 * @param setting The specified setting
	 * @return True if the specified setting is off 
	 */
	inline bool IsSettingOff(SIM_SETTING setting) {
		return !GetSetting(setting);
	}
	
	/**
     * @param setting The specified setting
	 * @return True if the specified setting is on 
	 */
	inline bool IsSettingOn(SIM_SETTING setting) {
		return GetSetting(setting);
	}

	/** @return True if the serial instance can read data */
	inline bool SerialActive() {
		return serialInstance->CanRead();
	}

	/**
	 * @return True if data can be read from serial and
	 * The USE_SERIAL setting is on
	 */
	inline bool SerialEnabled() { 
		return GetSetting(USE_SERIAL) && SerialActive();
	}
	/**
	 * @brief Updates the simulation; camera, rocket, serial etc
	 * @param deltaTime time between frames
	 */
	void Update(float deltaTime);

	/**
	 * @brief Draws the simulation
	 * @param deltaTime time between frames
	 */
	void Draw(float deltaTime);

private:

	/** @brief List of settings for the simulation */
	bool settings[SIM_SETTING::SETTINGS_COUNT] = { false };

	/** @brief how fast the camera moves (tiles per second) */
	float camera_speed = 5.0f;

	/** @brief internal camera struct */
	Camera3D cam;

	/** @brief Internal instance to get serial input */
	ArduinoSerial* serialInstance;

	/** @brief The rocket instance used by the simulation */
	Rocket* rocketInstance;

	/** @brief Override value to the orientation of the rocket */
	Cardanian3 rocket_orientation = { 0.0f, 0.0f, 0.0f };

	/** @brief Override rotations (radians) to canard fin rotations */
	float canard_rotations[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	/** @brief Canard rotation speed in Radians Per Second */
	float canardRPS = Rocket::DEFAULT_CANARD_RPS;

	/**
	 * @brief Used to get a memory pointer to the specific setting
	 * This is meant to be used by the SimGui as the GuiCheckBox requires
	 * a memory pointer to a boolean in order to set the setting
	 * @param setting The specified setting
	 * @return Memory pointer to the specified setting
	 */
	inline bool* GetSettingPtr(SIM_SETTING setting) {
		return &settings[setting];
	}
};


