#pragma once
#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <iomanip>
#include <windows.h>
#include "Fyredon/Other/pch.h"

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

#endif