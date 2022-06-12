#pragma once
struct GUIConfig{
	int AppWidth = 0;
	int AppHeight = 0;

	// menu
	const int menuHeight = 30;

	// leftBar
	int leftBar_width = 0;
	int leftBar_height = 0;
	const float leftBar_width_percent = 0.2;
	const int leftBar_width_min = 30;
	const int leftBar_width_max = 300;
	const float leftBar_height_percent = 0.7;
	const int leftBar_height_min = 30;
	//const int leftBar_height_max = 900;

	// rightBar
	int rightBar_width = 0;
	int rightBar_height = 0;
	const float rightBar_width_percent = 0.2;
	const int rightBar_width_min = 30;
	const int rightBar_width_max = 500;
	const float rightBar_height_percent = 0.7;
	const int rightBar_height_min = 30;
	const int rightBar_height_max = 500;

	// render widget offset
	const int xOffset = 20;
	const int yOffset = 60;
};

