// Made by Dominick O'Hara
// April 30, 2024
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>

#include "SimGui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


#include "Sim.h"
#include "raygui.h/../../styles/cyber/style_cyber.h" 

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// static floats must be defined in src .cpp file
const float SimGui::WINDOW_INSET = 24.0f;
const float SimGui::MAIN_PANEL_WIDTH = 180.0f;
const float SimGui::MAIN_PANEL_INSET = 4.0f;
const float SimGui::ELEM_SEPERATION = 4.0f;
const float SimGui::PANEL_SEPERATION = 8.0f;

const float SimGui::PANEL_HEADER_SIZE = 24.0f;
const float SimGui::RADIAN_PANEL_HEIGHT = 42.0f;


SimGui::SimGui(Sim* simPtr) : sim_ptr(simPtr)
{
	// initalize radian sliders
	rollSlider.prefix = "Roll";
	rollSlider.radians_ptr = &sim_ptr->rocket_orientation.roll;

	pitchSlider.prefix = "Pitch";
	pitchSlider.radians_ptr = &sim_ptr->rocket_orientation.pitch;

	yawSlider.prefix = "Yaw";
	yawSlider.radians_ptr = &sim_ptr->rocket_orientation.yaw;

	// radian sliders for fin rotation
	for (int i = 0; i < 4; i++) {
		canardRotSliders[i].prefix = canardLabels[i];
		canardRotSliders[i].radians_ptr = sim_ptr->canard_rotations + i;
	}

	canardRPSSlider.prefix = "Fin Rot / Sec";
	canardRPSSlider.radians_ptr = &sim_ptr->canardRPS;
}


const char* SimGui::GetAngleDisplayText(const char* prefix, float radians, bool use_degrees)
{
	char prefixStr[32];
	strcpy_s(prefixStr, prefix);
	float conversion = (use_degrees) ? RAD2DEG : 1.0f;
	const char* angleLabel = (use_degrees) ? "Deg" : "Rad";
	return TextFormat(
		"%s: %03.1f %s",
		prefixStr,
		radians * conversion,
		angleLabel
	);
}

void SimGui::RadianSlider_Draw(
	RadianSlider& radianSlider,
	bool use_degrees
)
{
	// refrence vars for  convience
	Rectangle& bounds = radianSlider.bounds;
	float*& radians_ptr = radianSlider.radians_ptr;
	char (&textboxText)[32] = radianSlider.textboxText;
	bool& textboxEditMode = radianSlider.textboxEditMode;


	// calculate stuff and create display text
	float conversion = (use_degrees) ? RAD2DEG : 1.0f;
	const char* displayText = GetAngleDisplayText(
		radianSlider.prefix,
		*radians_ptr,
		use_degrees
	);
	
	// draw radian slider group box
	bounds.height = RADIAN_PANEL_HEIGHT;
	GuiGroupBox(bounds, displayText);
	
	// apply inset
	bounds.x += MAIN_PANEL_INSET;
	bounds.width -= MAIN_PANEL_INSET * 2;
	
	// draw the textbox portion and get textbox input
	bounds.height = 16.0f;
	bounds.y += 8.0f;
	if (textboxEditMode == false) {
		char txt[32] = { 0 };
		sprintf_s(txt, "%04.1f", *(radians_ptr) * conversion);
		memcpy(textboxText, txt, 32);
	}
	else {
		sscanf_s(textboxText, "%f", radians_ptr);
		*(radians_ptr) /= conversion;
	}
	if (GuiTextBox(bounds, textboxText, 32, textboxEditMode)) {
		textboxEditMode = !textboxEditMode;
	}
	
	// draw the slider
	bounds.y += bounds.height + 4.0f;
	bounds.height = 8.0f;
	GuiSlider(bounds, "", "", radians_ptr, 0.0f, PI * 2);
}


void SimGui::FinRotationSliderPanel(
	Rectangle bounds,
	RadianSlider sliders[4],
	bool use_degrees
) {

	GuiPanel(bounds, "Fin Rotations");
	// prep bounds for use in drawing sliders
	bounds.x += MAIN_PANEL_INSET;
	bounds.width -= MAIN_PANEL_INSET * 2;
	bounds.height = RADIAN_PANEL_HEIGHT;
	bounds.y += PANEL_HEADER_SIZE + PANEL_SEPERATION;
	for (int i = 0; i < 4; i++)
	{
		sliders[i].bounds = bounds;
		RadianSlider_Draw(sliders[i], use_degrees);
		bounds.y += RADIAN_PANEL_HEIGHT + PANEL_SEPERATION;
	}

}


void SimGui::OrientationSliderPanel(
	Rectangle bounds, 
	RadianSlider& rollSlider,
	RadianSlider& pitchSlider,
	RadianSlider& yawSlider,
	bool use_degrees
) {

	GuiPanel(bounds, "Rocket Orientation");
	// add inset
	bounds.x += MAIN_PANEL_INSET;
	bounds.width -= MAIN_PANEL_INSET * 2;
	// create radian sliders
	bounds.y += 24.0f + PANEL_SEPERATION;
	bounds.height = RADIAN_PANEL_HEIGHT;
	rollSlider.bounds = bounds;
	RadianSlider_Draw(rollSlider, use_degrees);
	bounds.y += RADIAN_PANEL_HEIGHT + PANEL_SEPERATION;
	pitchSlider.bounds = bounds;
	RadianSlider_Draw(pitchSlider, use_degrees);
	bounds.y += RADIAN_PANEL_HEIGHT + PANEL_SEPERATION;
	yawSlider.bounds = bounds;
	RadianSlider_Draw(yawSlider, use_degrees);
}



void SimGui::UpdateAndDraw(float deltaTime)
{
	GuiLoadStyleCyber();
	
	Rectangle panelRect = {
		WINDOW_INSET,
		WINDOW_INSET,
		MAIN_PANEL_WIDTH,
		CALC_PANEL_HEIGHT(
			PANEL_HEADER_SIZE,
			Sim::SETTINGS_COUNT,
			PANEL_HEADER_SIZE,
			ELEM_SEPERATION
		)
	};

	// refrences to reduce amount of typing
	float(&canard_rotations)[4] = sim_ptr->canard_rotations;
	Cardanian3& rocket_orientation = sim_ptr->rocket_orientation;
	//float& canardRPS = sim_ptr->canardRPS;

	GuiPanel(panelRect, "Settings");
	Rectangle settingsRect = panelRect;

	// the check box is equal to the panel header size
	settingsRect.x += MAIN_PANEL_INSET;
	settingsRect.width = PANEL_HEADER_SIZE;
	settingsRect.height = PANEL_HEADER_SIZE;
	settingsRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
	
	GuiCheckBox(
		settingsRect,
		"Use Degrees", 
		sim_ptr->GetSettingPtr(Sim::USE_DEGREES)
	);
	
	settingsRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
	GuiCheckBox(
		settingsRect,
		"Display Rocket Axis", 
		sim_ptr->GetSettingPtr(Sim::DISPLAY_ROCKET_AXIS)
	);
	
	settingsRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
	GuiCheckBox(
		settingsRect, 
		"Display World Axis", 
		sim_ptr->GetSettingPtr(Sim::DISPLAY_WORLD_AXIS)
	);
	
	settingsRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
	GuiCheckBox(
		settingsRect, 
		"Instant Fin Rotation", 
		sim_ptr->GetSettingPtr(Sim::INSTANT_FIN_ROT)
	);
	
	settingsRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
	GuiCheckBox(
		settingsRect,
		"Use Stablization Sys",
		sim_ptr->GetSettingPtr(Sim::USE_STABL_SYS)
	);
	
	settingsRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
	GuiCheckBox(
		settingsRect, 
		"Use Serial",
		sim_ptr->GetSettingPtr(Sim::USE_SERIAL)
	);

	// draw serial port selector (if applicable)
	if (sim_ptr->GetSetting(Sim::USE_SERIAL) && !sim_ptr->SerialActive()) {
		Rectangle SerialOpenRect;
		SerialOpenRect.width = 160.0f;
		SerialOpenRect.height = CALC_PANEL_HEIGHT(PANEL_HEADER_SIZE, 3, 16, ELEM_SEPERATION);//PANEL_HEADER_SIZE + ELEM_SEPERATION * 3 + 32;
		SerialOpenRect.x = GetScreenWidth() / 2 - 50;
		SerialOpenRect.y = GetScreenHeight() / 2 - 27;
		GuiPanel(SerialOpenRect, "Serial port");

		SerialOpenRect.y += PANEL_HEADER_SIZE + ELEM_SEPERATION;
		SerialOpenRect.height = 16;
		SerialOpenRect.width -= MAIN_PANEL_INSET * 2;
		SerialOpenRect.x += MAIN_PANEL_INSET;
		
		if (GuiTextBox(SerialOpenRect, serial_port_entry, 32, textboxSerialPortEditMode))
			textboxSerialPortEditMode = !textboxSerialPortEditMode;

		SerialOpenRect.y += ELEM_SEPERATION + 16;
		if (GuiTextBox(SerialOpenRect, serial_baud_entry, 32, textboxSerialBaudEditMode))
			textboxSerialBaudEditMode = !textboxSerialBaudEditMode;
		
		SerialOpenRect.y += SerialOpenRect.height + ELEM_SEPERATION;
		if (GuiButton(SerialOpenRect, "Open")) {
			if (ArduinoSerial::PortAvailable(std::string(serial_port_entry))) {
				int baud = atoi(serial_baud_entry);
				if (sim_ptr->serialInstance->Open(serial_port_entry, baud)) {
					std::cout << "Successfuly opened serial port " << serial_port_entry << std::endl;
				}
			}

		}
	}

	// draw reset button
	panelRect.y += panelRect.height;
	panelRect.y += PANEL_SEPERATION;
	panelRect.height = PANEL_HEADER_SIZE;
	if (GuiButton(panelRect, "Reset Rocket")) {
		rocket_orientation = { 0.0f, 0.0f, 0.0f };
		canard_rotations[0] = 0.0f;
		canard_rotations[1] = 0.0f;
		canard_rotations[2] = 0.0f;
		canard_rotations[3] = 0.0f;
		sim_ptr->canardRPS = Sim::DEFAULT_CANARD_RPS;
		float newCanardRotations[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		sim_ptr->rocketInstance->SetAllCanardRotation(newCanardRotations);
	}

	// draw input for fin RPS
	if (sim_ptr->IsSettingOff(Sim::INSTANT_FIN_ROT)) {
		panelRect.y += panelRect.height;
		panelRect.y += PANEL_SEPERATION;
		panelRect.height = RADIAN_PANEL_HEIGHT;
		canardRPSSlider.bounds = panelRect;
		RadianSlider_Draw(canardRPSSlider, sim_ptr->GetSetting(Sim::USE_DEGREES));
	}

	// draw orientation sliders
	if (sim_ptr->IsSettingOff(Sim::USE_SERIAL)) {
		panelRect.y += panelRect.height;
		panelRect.y += PANEL_SEPERATION;
		panelRect.height = CALC_PANEL_HEIGHT(
			PANEL_HEADER_SIZE,
			3,
			RADIAN_PANEL_HEIGHT,
			PANEL_SEPERATION
		);
		OrientationSliderPanel(
			panelRect,
			rollSlider,
			pitchSlider,
			yawSlider,
			sim_ptr->GetSetting(Sim::USE_DEGREES)
		);
	}
	else // draw orientation display for serial
	{
		panelRect.y += panelRect.height;
		panelRect.y += PANEL_SEPERATION;
		panelRect.height = CALC_PANEL_HEIGHT(
			ELEM_SEPERATION,
			3,
			12,
			ELEM_SEPERATION
		);
		GuiGroupBox(panelRect, "Rocket Orientation (Serial)");

		Rectangle displayRect = panelRect;
		displayRect.height = PANEL_HEADER_SIZE;
		displayRect.x += MAIN_PANEL_INSET;
		displayRect.width -= MAIN_PANEL_INSET * 2;
		
		// roll display
		GuiLabel(
			displayRect, 
			GetAngleDisplayText(
				"Roll", 
				rocket_orientation.roll, 
				sim_ptr->GetSetting(Sim::USE_DEGREES)
			)
		);

		// pitch display
		displayRect.y += 12.0f + ELEM_SEPERATION;
		GuiLabel(
			displayRect, 
			GetAngleDisplayText(
				"Pitch", 
				rocket_orientation.pitch, 
				sim_ptr->GetSetting(Sim::USE_DEGREES)
			)
		);

		// yaw display
		displayRect.y += 12.0f + ELEM_SEPERATION;
		GuiLabel(
			displayRect, 
			GetAngleDisplayText(
				"Yaw", 
				rocket_orientation.yaw, 
				sim_ptr->GetSetting(Sim::USE_DEGREES)
			)
		);
	}

	// draw canard radian sliders
	if (sim_ptr->IsSettingOff(Sim::USE_STABL_SYS)) {
		panelRect.y += panelRect.height;
		panelRect.y += PANEL_SEPERATION;
		panelRect.height = CALC_PANEL_HEIGHT(
			PANEL_HEADER_SIZE,
			4,
			RADIAN_PANEL_HEIGHT,
			PANEL_SEPERATION
		);
		FinRotationSliderPanel(
			panelRect,
			canardRotSliders,
			sim_ptr->GetSetting(Sim::USE_DEGREES)
		);
	}
	else // draw canard rotation display for stablization system
	{
		panelRect.y += panelRect.height;
		panelRect.y += PANEL_SEPERATION;
		panelRect.height = CALC_PANEL_HEIGHT(
			ELEM_SEPERATION,
			4,
			12,
			ELEM_SEPERATION
		);
		GuiGroupBox(panelRect, "Fin Rotation (Stabl. Sys)");
		Rectangle displayRect = panelRect;
		displayRect.height = PANEL_HEADER_SIZE;
		displayRect.x += MAIN_PANEL_INSET;
		displayRect.width -= MAIN_PANEL_INSET * 2;
		for (int i = 0; i < 4; i++) {
			GuiLabel(
				displayRect, 
				GetAngleDisplayText(
					canardLabels[i], 
					canard_rotations[i], 
					sim_ptr->GetSetting(Sim::USE_DEGREES)
				)
			);
			displayRect.y += 12.0f + ELEM_SEPERATION;
		}

	}
}