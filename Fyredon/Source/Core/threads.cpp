// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Threads.cpp: Thread manager general : Isiash
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Core/Threads.h"

#include "Fyredon/Core/RandomGenerator.h"

void nonmembertest() {
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "non member test " << i << std::endl;
	}
}

Fyredon::ThreadManager::ThreadManager() {
	// Create Job
	job j1;
	j1.id = 0;
	j1.data = NULL;
	j1.executed = 0;

	// Use member function from a different class
	//TODO Need to use templates? to generalise function input/outputs
	RandomGenerator prng = RandomGenerator(1);
	j1.task = [prng]() {prng.printDouble(); };// bind(&test_job);
	add_job(j1, game_job_list);
	
	start_all_threads();
};

Fyredon::ThreadManager::~ThreadManager() {
	kill_all_threads();
};


void Fyredon::ThreadManager::start_all_threads() {
	//TODO implement mutex (more information about data used by separate threads is needed)
	start_debug_thread();
	start_game_thread();
	start_render_thread();
	start_physics_thread();
}

void Fyredon::ThreadManager::start_debug_thread() {
	debug_thread = std::thread(&Fyredon::ThreadManager::dummy_debug_loop, this);
	debug_thread.join();
}

void Fyredon::ThreadManager::start_game_thread() {
	game_thread = std::thread(&Fyredon::ThreadManager::dummy_game_loop, this);
	game_thread.join();


}

void Fyredon::ThreadManager::start_render_thread() {
	render_thread = std::thread(&Fyredon::ThreadManager::dummy_render_loop, this);
	render_thread.join();
}

void Fyredon::ThreadManager::start_physics_thread() {
	physics_thread = std::thread(&Fyredon::ThreadManager::dummy_physics_loop, this);
	physics_thread.join();
}


void Fyredon::ThreadManager::kill_all_threads() {
	kill_debug_thread();
	kill_game_thread();
	kill_render_thread();
	kill_physics_thread();
}

void Fyredon::ThreadManager::kill_debug_thread() {
	debug_job_list.clear();
	//TODO terminate thread
}

void Fyredon::ThreadManager::kill_game_thread() {
	game_job_list.clear();
	//TODO terminate thread
}

void Fyredon::ThreadManager::kill_render_thread() {
	render_job_list.clear();
	//TODO terminate thread
}

void Fyredon::ThreadManager::kill_physics_thread() {
	physics_job_list.clear();
	//TODO terminate thread
}


void Fyredon::ThreadManager::test_job() {
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "test" << std::endl;
	}
}

void Fyredon::ThreadManager::dummy_debug_loop() {
	for (int i = 0; i < 5; i++)
		std::cout << "debug " << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Fyredon::ThreadManager::dummy_game_loop() {
	for (int i = 0; i < 5; i++)
		std::cout << "game " << std::endl;

	for (int i = 0; i < game_job_list.size(); i++) {
		std::cout << i << " i\n";
		std::cout << "id " << game_job_list[i].id << std::endl;
	}

	//while (true) {
	std::cout << "this loop " << std::endl;
		if (game_job_list.size() > 0)
		{
			std::cout << game_job_list.size() << " size " << std::endl;
			job current_job = game_job_list.front();
			invoke(current_job.task);
			game_job_list.front().executed = 1;
			remove_first_job(game_job_list);
			//game_job_list.erase(game_job_list.begin());
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	//}
}

void Fyredon::ThreadManager::dummy_render_loop() {
	for (int i = 0; i < 5; i++)
		std::cout << "render " << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Fyredon::ThreadManager::dummy_physics_loop() {
	for (int i = 0; i < 5; i++)
		std::cout << "physics " << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

unsigned int Fyredon::ThreadManager::getJobID(job j) {
	return j.id;

}

void Fyredon::ThreadManager::add_job(job j, std::vector<job>& list) {
	// Get a sorted list of all ids
	std::vector<unsigned int> all_ids;
	for (size_t i = 0; i < list.size(); i++) {
		all_ids.push_back(list.at(i).id);
	}
	sort(all_ids.begin(), all_ids.end());

	// Find the lowest missing id
	unsigned int lowest_missing_id;
	for (unsigned int i = 0; i < all_ids.size(); i++) {
		if (i != all_ids[i]) {
			lowest_missing_id = i;
		}
	}


	// if the id provided is not unique then replace with the lowest missing id
	unsigned int current_job_id = j.id;
	for (size_t i = 0; i < list.size(); i++) {
		unsigned int id = getJobID(list.at(i));
		if (id == current_job_id) {
			current_job_id = lowest_missing_id;
			break;
		}
	}
	j.id = current_job_id;

	list.push_back(j);
	std::cout << "List size is now " << list.size() << std::endl;
}

void Fyredon::ThreadManager::remove_first_job(std::vector<job>& list) {
	list.erase(list.begin());
}
