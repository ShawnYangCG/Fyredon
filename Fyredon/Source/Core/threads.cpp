#include <threads.h>

ThreadManager::ThreadManager() {
	start_all_threads();

	job j1;
	j1.id = 0;
	j1.data = NULL;
	j1.executed = 0;
	//void(*func1)(void*) = dummy_debug_loop;

	/*int* = 1;
	void* data = ;
	j1.data = data;*/

	void (*test)(void*) = &test_job;
	j1.function = test;

	cout << "game list size " << game_job_list.size() << endl;
	add_job(j1, game_job_list);
	cout << "game list size " << game_job_list.size() << endl;

	
	remove_job(j1, game_job_list);
	cout << "game list size " << game_job_list.size() << endl;


	remove_job(j1, game_job_list);
	cout << "game list size " << game_job_list.size() << endl;



};

ThreadManager::~ThreadManager() {
	kill_all_threads();
};


void ThreadManager::start_all_threads() {
	//TODO implement mutex (more information about data used by separate threads is needed)
	start_debug_thread();
	start_game_thread();
	start_render_thread();
	start_physics_thread();
}

void ThreadManager::start_debug_thread(){
	debug_thread = new thread(&ThreadManager::dummy_debug_loop, this);
}

void ThreadManager::start_game_thread() {
	game_thread = new thread(&ThreadManager::dummy_game_loop, this);
}

void ThreadManager::start_render_thread() {
	render_thread = new thread(&ThreadManager::dummy_render_loop, this);
}

void ThreadManager::start_physics_thread() {
	physics_thread = new thread(&ThreadManager::dummy_physics_loop, this);
}


void ThreadManager::kill_all_threads() {
	kill_debug_thread();
	kill_game_thread();
	kill_render_thread();
	kill_physics_thread();
}

void ThreadManager::kill_debug_thread() {

}

void ThreadManager::kill_game_thread() {

}

void ThreadManager::kill_render_thread() {

}

void ThreadManager::kill_physics_thread() {

}


void ThreadManager::dummy_debug_loop() {
	for (int i = 0; i < 5; i++)
		cout << "debug " << endl;
	this_thread::sleep_for(chrono::seconds(1));
}

void ThreadManager::dummy_game_loop() {
	for (int i = 0; i < 5; i++)
		cout << "game " << endl;
	this_thread::sleep_for(chrono::seconds(1));
}

void ThreadManager::dummy_render_loop() {
	for (int i = 0; i < 5; i++)
		cout << "render " << endl;
	this_thread::sleep_for(chrono::seconds(1));
}

void ThreadManager::dummy_physics_loop() {
	for (int i = 0; i < 5; i++)
		cout << "physics " << endl;
	this_thread::sleep_for(chrono::seconds(1));
}


void test_job(void* data) {
	for (size_t i = 0; i < 10; i++)
	{
		cout << "test" << endl;
	}
}

unsigned int ThreadManager::getJobID(job j) {
	return j.id;

}

void ThreadManager::add_job(job j, vector<job>& list) {
	// Get a sorted list of all ids
	vector<unsigned int> all_ids;
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
	for (size_t i = 0; i < list.size(); i++){
		unsigned int id = getJobID(list.at(i));
		if (id == current_job_id) {
			current_job_id = lowest_missing_id;
			break;
		}
	}
	j.id = current_job_id;

	list.push_back(j);
}

void ThreadManager::remove_job(job j, vector<job>& list) {
	int i;
	for (i = 0; i < list.size(); i++)	{
		if (j.id == list[i].id) {
			list.erase(list.begin() + i);
		}
	}

	if (i == list.size())
		cout << "The job with the ID " << j.id << " cannot be found" << endl;
}
