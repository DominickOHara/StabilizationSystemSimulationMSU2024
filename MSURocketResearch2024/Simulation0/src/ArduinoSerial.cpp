// Made by Dominick O'Hara
// April 30, 2024
#include "ArduinoSerial.h"
#include <iostream>

const float DEG2RAD = PI / 180.0f;

bool ArduinoSerial::Open(const std::string& COM_PORT, uint32_t baud)
{
	serialObj = new serial::Serial(COM_PORT, 9600U, serial::Timeout::simpleTimeout(10), serial::eightbits, serial::parity_even, serial::stopbits_one);
	return true;
}

ArduinoSerial::~ArduinoSerial() {
	if (serialObj != nullptr) {
		serialObj->close();
		delete serialObj;
	}
}

bool ArduinoSerial::PortAvailable(const std::string& COM_PORT)
{
	bool canCreateSerial = false;
	for (serial::PortInfo portInfo : serial::list_ports()) {
		if (portInfo.port == COM_PORT)
			return true;
	}
	return false;
}


void ArduinoSerial::ReadFloats(size_t count, int floatChars, bool serialUsesDeg)
{
	if (!CanRead())
		return;

	// get serial data
	std::string data;
	size_t bytes_read = serialObj->readline(data);//(DATA_NUM_SIZE*3);
	const char* data_cstr = data.c_str();
	std::cout << "Reading data. Bytes Read: " << bytes_read << std::endl;
	std::cout << "Bytes available: " << serialObj->available() << std::endl;
	
	// extract floats from data
	char* floatStr = new char[floatChars + 1]; // +1 is to account for null terminator
	memset(floatStr, 0, floatChars + 1); // get rid of junk values
	
	int charsLeft = data.length(); // will not include null terminator '\0'
	for (int i = 0; i < count && charsLeft >= floatChars; i++, charsLeft -= floatChars) {
		memcpy(floatStr, data_cstr + i * floatChars, floatChars);
		float radianMeasure = atof(floatStr);
		// radian measure can be in degrees if serial uses degrees
		radianMeasure = (serialUsesDeg)? radianMeasure * DEG2RAD : radianMeasure;
		std::cout << "float " << i << " " << radianMeasure << std::endl;
		float_data[i] = radianMeasure;
	}
	
	delete[] floatStr;

}


void ArduinoSerial::Update(float deltaTime)
{
	ReadFloats(7);
}

