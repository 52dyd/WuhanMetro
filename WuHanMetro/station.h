#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<list>
#include"edges.h"
#include"metro.h"

class People;
class Metros;
class Stations
{
public:
	Stations(int no, int max, std::string name)
	{
		this->no = no;
		this->max_people = max;
		this->name = name;
	}
	~Stations() {}

	int get_line() {
		return this->no / 100;
	}

	int get_no() {
		return this->no;
	}

	std::string get_name() {
		return this->name;
	}

	bool get_person(People* p) {
		if (num_people == max_people) return false;
		num_people++;
		now_people.push_back(p);
		return true;
	}

	bool out_person(People* p) {
		std::list<People*>::iterator tmp;
		for (tmp = now_people.begin(); tmp != now_people.end(); tmp++) {
			if (*tmp == p) { //found it and delete
				tmp = now_people.erase(tmp);
				num_people--;
				return true;
			}
		}
		return false;
	}

	bool get_metro(Metros* m) {
		std::vector<Metros*>::iterator tmp;
		for (tmp = now_metros.begin(); tmp != now_metros.end(); tmp++) {
			if ((*tmp)->get_end_station()->get_no() == (*m).get_end_station()->get_no()) return false;
		}
		this->now_metros.push_back(m);
		return true;
	}

	bool out_metro(Metros* m) {
		std::vector<Metros*>::iterator tmp;
		for (tmp = now_metros.begin(); tmp != now_metros.end(); tmp++) {
			if ((*tmp) == m) {
				tmp = now_metros.erase(tmp);
				return true;
			}
		}
		return false;
	}

	Metros* has_metro(int l, Stations* e) {
		if (l != this->get_line()) return NULL;
		std::vector<Metros*>::iterator tmp;
		for (tmp = now_metros.begin(); tmp != now_metros.end(); tmp++) {
			if ((*tmp)->get_end_station()->get_no() == e->get_no()) return (*tmp);
		}
		return NULL;
	}

	int transferring_time() { //min
		return (int)(((float)num_people / (float)max_people) * 10);
	}

	bool edge_push_back(Edges* e) {
		edge.push_back(*e);
		return true;
	}

	std::vector<Stations*> get_transfer_station() {
		std::vector<Stations*> tmp_stations;
		tmp_stations.clear();
		for (auto iter : edge) {
			if (iter.len == 0) tmp_stations.push_back(iter.end);
		}
		return tmp_stations;
	}

	Stations* get_transfer_station(int line) {
		for (auto iter : edge) {
			if (iter.len == 0 && iter.end->get_line() == line) return iter.end;
		}
		return NULL;
	}

	bool tf_nxt_station(Stations* station) {
		for (auto i : this->edge) {
			if (i.end->get_no() == station->get_no()) return true;
		}
		return false;
	}

	int get_num_people() {
		return this->num_people;
	}

private:
	int no;
	int num_people;
	int max_people;
	std::string name;
	std::list<People*> now_people;
	std::vector<Metros*> now_metros;
	std::vector<Edges> edge;
};