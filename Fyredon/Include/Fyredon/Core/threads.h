#ifndef THREADS_H
#define THREADS_H

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <algorithm>  

using namespace std;

struct job {
	unsigned int id;
	void  (*function)(void*);   
	void* data;                 
	int    executed;            
};


void test_job(void* data);

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

	vector<job> debug_job_list;
	vector<job> game_job_list;
	vector<job> render_job_list;
	vector<job> physics_job_list;

	void add_job(job j, vector<job>& list);
	void remove_job(job j, vector<job>& list);

	unsigned int getJobID(job j);


	void dummy_debug_loop();
	void dummy_game_loop();
	void dummy_render_loop();
	void dummy_physics_loop();


private:
	thread* debug_thread = NULL;
	thread* game_thread = NULL;
	thread* render_thread = NULL;
	thread* physics_thread = NULL;

	//TODO mutex causing slient deletion of function. cannot copy class
	//mutex mutual_exclusion;
};

#endif