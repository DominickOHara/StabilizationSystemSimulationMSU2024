// Made by Dominick O'Hara
// April 30, 2024
#pragma once
#include "raylib.h"


class SimGui {

public:
	/** @brief Whitespace between edge of window and UI. */
	const static float WINDOW_INSET;

	/** @brief Width of main panel in pixels. */
	const static float MAIN_PANEL_WIDTH;

	/**
	 * @brief Whitespace between the edge of elements within
	 * the main panel and the elements inside of these elements.
	 */
	const static float MAIN_PANEL_INSET;

	/** @brief Whitespace between each element in main panel. */
	const static float ELEM_SEPERATION;

	/** @brief Whitespace between edge of window and UI. */
	const static float PANEL_SEPERATION;

	/**
	 * @brief Size of the panel header. This is where the title
	 * of the panels are drawn. DO NOT configure this value as
	 * it is meant to stay 24.0f as that is the constant size
	 * given in RayGui.
	 */
	const static float PANEL_HEADER_SIZE;

	/** @brief Height of radian slider in pixels */
	const static float RADIAN_PANEL_HEIGHT;

	/**
	 * @brief A compile-time utility function to calculate
	 * the height of a panel containing x internal elements and
	 * their respective sizes and seperation.
	 *
	 * @param HEADER_SIZE size of header of panel.
	 * @param ELEMS amount of elements within the panel.
	 * @param ELEM_HEIGHT height of each element.
	 * @param ELEM_SEP whitespace between each element.
	 * @return Size of the panel in pixels.
	 */
	constexpr float CALC_PANEL_HEIGHT(
		float HEADER_SIZE,
		int ELEMS,
		float ELEM_HEIGHT,
		float ELEM_SEP
	) {
		return HEADER_SIZE + ELEMS * (ELEM_HEIGHT + ELEM_SEP) + ELEM_SEP;
	}

	SimGui(class Sim* sim_ptr);
	
	/**
	 * @brief Updates and draws the simulation UI
	 * the reason for this is because the library uses
	 * immediate mode UI, the inputs from the UI are recieved
	 * whenever the UI is drawn. Therefore, the updates and
	 * draws of the simulation UI must be done at the same time. 
	 * 
	 * @param deltaTime time between frames
	 */
	void UpdateAndDraw(float deltaTime);

private:

	/** @brief A constant pointer to the simulation UI is attached to */
	class Sim* const sim_ptr;

	/**
	 * @brief Structure to store the state of a slider
	 * which manages angles
	 *
	 * @param bounds size and position of slider; only width of size is concidered
	 * @param prefix text before angle measurement
	 * @param radians_ptr memory pointer to value it edits
	 * @param textboxText internal data of textbox
	 * @param textboxEditMode internal state of textbox
	 */
	struct RadianSlider {
		Rectangle bounds;
		const char* prefix = { 0 };
		float* radians_ptr;
		char textboxText[32] = { 0 };
		bool textboxEditMode = false;
	};

	void RadianSlider_Draw(RadianSlider& radianSlider, bool use_degrees);

	// sliders
	RadianSlider rollSlider;
	RadianSlider pitchSlider;
	RadianSlider yawSlider;
	char canardLabels[4][6] = { "Fin 1", "Fin 2", "Fin 3", "Fin 4" };
	RadianSlider canardRotSliders[4] = {};
	RadianSlider canardRPSSlider;

	char serial_port_entry[32] = "COM3";
	bool textboxSerialPortEditMode = false;
	char serial_baud_entry[32] = "9600";
	bool textboxSerialBaudEditMode = false;

	void OrientationSliderPanel(
		Rectangle bounds,
		RadianSlider& rollSlider,
		RadianSlider& pitchSlider,
		RadianSlider& yawSlider,
		bool use_degrees
	);

	void FinRotationSliderPanel(
		Rectangle bounds,
		RadianSlider sliders[4],
		bool use_degrees
	);

	/**
	 * @brief Creates text which displays radians given a prefix
	 * This also supports auto conversion to degrees
	 * 
	 * @param prefix The prefix to the angle display
	 * @param radians The radian amount used for the text
	 * @param use_radians weather or not to convert the radians into degrees
	 * @return Null-terminated C String (const char* with last char being \0) 
	 * being the angle display text
	 */
	const char* GetAngleDisplayText(const char* prefix, float radians, bool use_degrees);
};

