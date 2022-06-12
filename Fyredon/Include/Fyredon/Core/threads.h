// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Threads.h: Thread manager general : Isiash
=============================================================================*/
#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <algorithm>  
#include <functional> 
#include <list>

//using namespace std;
//using namespace std::placeholders;

namespace Fyredon {
//template<typename T>
struct job {
	unsigned int id;
	void  (*function)(void*);
	void* data;
	int    executed;

	std::function<void()> task;
	//std::function<T(void())> task;
	//[&] { some_obj.some_method(some_arg); };
};

//void test_job(void* data);

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	// State Setup
	void start_all_threads();
	void start_debug_thread();
	void start_game_thread();
	void start_render_thread();
	void start_physics_thread();

	void kill_all_threads();
	void kill_debug_thread();
	void kill_game_thread();
	void kill_render_thread();
	void kill_physics_thread();

	std::vector<job> debug_job_list;
	std::vector<job> game_job_list;
	std::vector<job> render_job_list;
	std::vector<job> physics_job_list;

	void add_job(job j, std::vector<job>& list);
	void remove_first_job(std::vector<job>& list);

	unsigned int getJobID(job j);


	void dummy_debug_loop();
	void dummy_game_loop();
	void dummy_render_loop();
	void dummy_physics_loop();

	void test_job();


private:
	std::thread debug_thread;
	std::thread game_thread;
	std::thread render_thread;
	std::thread physics_thread;

	//TODO mutex causing slient deletion of function. cannot copy class
	//mutex mutual_exclusion;
};
}

