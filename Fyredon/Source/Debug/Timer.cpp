// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Timer.cpp: timer manager general : Shawn
=============================================================================*/
#include "Fyredon/Other/pch.h"


#include <Fyredon/Debug/Timer.h>

Fyredon::TimerManager::TimerManager()
{

}

Fyredon::TimerManager::~TimerManager()
{
}

void Fyredon::TimerManager::start_all_timer()
{
	start_animation_timer();
	start_physics_timer();
	start_logic_timer();
}

void Fyredon::TimerManager::start_animation_timer()
{
	for (int i = 0; i < this->animation_timer_list.size(); i++)
		this->timerRun(this->animation_timer_list[i]);
}

void Fyredon::TimerManager::start_physics_timer()
{
	for (int i = 0; i < this->physics_timer_list.size(); i++)
		this->timerRun(this->physics_timer_list[i]);
}

void Fyredon::TimerManager::start_logic_timer()
{
	for (int i = 0; i < this->logic_timer_list.size(); i++)
		this->timerRun(this->logic_timer_list[i]);
}

void Fyredon::TimerManager::stop_all_timer()
{
	stop_animation_timer();
	stop_physics_timer();
	stop_logic_timer();
}

void Fyredon::TimerManager::stop_animation_timer()
{
	for (int i = 0; i < this->animation_timer_list.size(); i++)
		this->timerStop(this->animation_timer_list[i]);
}

void Fyredon::TimerManager::stop_physics_timer()
{
	for (int i = 0; i < this->physics_timer_list.size(); i++)
		this->timerStop(this->physics_timer_list[i]);
}

void Fyredon::TimerManager::stop_logic_timer()
{
	for (int i = 0; i < this->logic_timer_list.size(); i++)
		this->timerStop(this->logic_timer_list[i]);
}

void Fyredon::TimerManager::add_timer(std::string type)
{
	timer newclock = { new Clock(),type,-1,false };
	if (type == "animation") {
		newclock.index = this->animation_timer_list.size();
		this->animation_timer_list.push_back(newclock);
	}
	else if (type == "physics"){
		newclock.index = this->physics_timer_list.size();
		this->physics_timer_list.push_back(newclock);
	}
	else if (type == "logic") {
		newclock.index = this->logic_timer_list.size();
		this->logic_timer_list.push_back(newclock);
	}
	else
		std::cout << "unknown type name: " << type << std::endl;
}

void Fyredon::TimerManager::remove_timer(timer t)
{
	if (t.type == "animation")
		this->animation_timer_list.erase(this->animation_timer_list.begin() + t.index);
	else if (t.type == "physics") 
		this->physics_timer_list.erase(this->animation_timer_list.begin() + t.index);
	else if (t.type == "logic") 
		this->logic_timer_list.erase(this->animation_timer_list.begin() + t.index);
}

Fyredon::timer Fyredon::TimerManager::getTimer(int index, std::string type)
{
	if (type == "animation")
		return this->animation_timer_list[index];
	else if (type == "physics")
		return this->physics_timer_list[index];
	else if (type == "logic")
		return this->logic_timer_list[index];
	else
		std::cout << "unknown type name: " << type << std::endl;
}

void Fyredon::TimerManager::timerRun(timer& t)
{
	t.pclock->run();
	t.IsRunning = true;
}

void Fyredon::TimerManager::timerStop(timer& t)
{
	t.pclock->stop();
	t.IsRunning = false;
}

std::string Fyredon::TimerManager::getTimerType(timer t)
{
	return t.type;
}
