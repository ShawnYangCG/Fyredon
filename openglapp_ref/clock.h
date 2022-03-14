#pragma once
#include <iostream>
#include <iomanip>
#include <windows.h>
class Clock
{
public:
	Clock();
	~Clock();
	void run();
	void stop();
private:
	bool isRuning;
	int hours;
	int min;
	int sec;
	void show();
	void tick();
};