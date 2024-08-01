// Made by Dominick O'Hara
// April 30, 2024
#pragma once

#include "serial/serial.h"
#include "Cardanian3.h"
#include <string>

const int DATA_NUM_SIZE = 8;
const int SERIAL_DATA_STR_SIZE = DATA_NUM_SIZE * 3 + 1;
class ArduinoSerial
{

public:
	//ArduinoSerial();
	~ArduinoSerial();

	/** @return true if COM port is present on system */
	static bool PortAvailable(const std::string& COM_PORT);

	void Update(float deltaTime);

	/** @return true if an internal serial object is not nullptr */
	inline bool CanRead() { return serialObj != nullptr; }
	bool Open(const std::string& COM_PORT, uint32_t baud = 9600);

	/** @return Cardanian3 struct storing the orientation sent over by serial */
	inline Cardanian3 GetOrientation()
	{ return { this->float_data[0], this->float_data[1], this->float_data[2] }; }

	/**
	 * @brief stores the canard fin rotations sent by serial to the provided
	 * float array
	 * @param rotation float array with 4 floats
	 */
	inline void GetCanardRotation(float rotation[4]) {
		rotation[0] = float_data[3];
		rotation[1] = float_data[4];
		rotation[2] = float_data[5];
		rotation[3] = float_data[6];
	}

	/**
	 * @brief reads the next n floats
	 * @param floatc amount of floats to read; default 7
	 * @param datasize amount of characters each float takes; default 8
	 */
	void ReadFloats(size_t floatc = 7, int datasize = 8, bool serialUsesDeg = false);
	
private:

	/** @brief Internal serial object to read from serial port */
	serial::Serial* serialObj = nullptr;

	/**
	 * @brief The data to be read from the serial. The first 3 floats
	 * are the orientation of the rocket in the order of Roll, Pitch, Yaw
	 * and the next 4 floats are the canard fin rotations.
	 */
	float float_data[7] = { 0.0f };

};

