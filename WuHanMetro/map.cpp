#include<thread>
#include"map.h"
#define NUM_CORE 1


void Map::nxt_scd(int core, int num_core) {
	//generate_person(NULL, NULL, 3200 / num_core);  //num of each min;
	//Generate_metro();
	auto i = this->metro.begin();

	int tmp1 = this->metro.size() * core / num_core;
	int tmp2 = tmp1;
	while (tmp1--) i++;
	tmp1 = tmp2;
	tmp2 = this->metro.size() * (core + 1) / num_core;
	for (; tmp1 < tmp2; tmp1++, i++)
		(*i)->nxt_scd();

	tmp1 = this->person.size() * core / num_core;
	tmp2 = tmp1;
	auto j = this->person.begin();
	while (tmp1--) j++;
	tmp1 = tmp2;
	tmp2 = this->person.size() * (core + 1) / num_core;
	for (; tmp1 < tmp2; j++, tmp1++) {
		(*j)->nxt_scd();
	}
	return;
	//this->now_time++;
}

void Map::nxt_scd_thread() {
	this->Generate_metro();
	for (int i = 0; i < NUM_CORE; i++) {
		auto tmp_thread = new std::thread(&Map::nxt_scd, this, i, NUM_CORE);
		tmp_thread->join();
	}
	for (auto i = this->metro.begin(); i != this->metro.end();) {
		if (!(*i)->get_live()) {
			i = this->metro.erase(i);
			continue;
		}
		i++;
	}
	for (auto i = this->person.begin(); i != this->person.end();) {
		if (!(*i)->tf_live()) {
			i = this->person.erase(i);
			continue;
		}
		i++;
	}
	this->now_time++;
}