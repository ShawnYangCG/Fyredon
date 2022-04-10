// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	timer.h: timer manager general : Shawn
=============================================================================*/
#pragma once

#include <time.h>
#include <thread>
#include <vector>
#include <string>
#include <Fyredon/Core/Clock.h>

namespace Fyredon {

struct timer {
Clock* pclock;
std::string type;
int index;
bool IsRunning;
};

class TimerManager
{
public:
	TimerManager();
	~TimerManager();

	// Start timer
	void start_all_timer();
	void start_animation_timer();
	void start_physics_timer();
	void start_logic_timer();

	// Stop timer
	void stop_all_timer();
	void stop_animation_timer();
	void stop_physics_timer();
	void stop_logic_timer();

	std::vector<timer> animation_timer_list;
	std::vector<timer> physics_timer_list;
	std::vector<timer> logic_timer_list;

	void add_timer(std::string type);
	void remove_timer(timer t);

	timer getTimer(int index, std::string type);
	void timerRun(timer& t);
	void timerStop(timer& t);

	std::string getTimerType(timer t);
};


}

