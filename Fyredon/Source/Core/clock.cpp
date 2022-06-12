// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Clock.h: [..] : ?
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include <Fyredon/Core/clock.h>

// Replace with PCH...
#include <iostream>
#include <iomanip>
#include <windows.h>

Fyredon::Clock::Clock()
{
	time_t t = time(NULL);
	tm ti;
	localtime_s(&ti, &t);

	hours = ti.tm_hour;
	min = ti.tm_min;
	sec = ti.tm_sec;
}

void Fyredon::Clock::run()
{
	this->isRuning = true;
	while (1)
	{
		if (this->isRuning == false)
			break;
		show();
		tick();
	}
}

void Fyredon::Clock::stop()
{
	this->isRuning = false;
}

void Fyredon::Clock::show()
{
	using namespace std;
	system("cls");
	cout << setw(2) << setfill('0') << hours << ": ";
	cout << setw(2) << setfill('0') << min << ": ";
	cout << setw(2) << setfill('0') << sec;
}
void Fyredon::Clock::tick()
{
	Sleep(1000);
	if (++sec == 60)
	{
		sec = 0;
		min += 1;
		if (min == 60)
		{
			min = 0;
			hours += 1;
			if (hours == 24)
			{
				hours = 0;
			}
		}
	}
}

Fyredon::Clock::~Clock()
{
}