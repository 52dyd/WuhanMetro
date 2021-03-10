#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<map>
#include"station.h"
#include"people.h"
#include"metro.h"
#include"answer.h"
#include"dest.h"

class Map
{
public:
	Map() {
		this->recode = false;
		int line;
		int max_people;
		std::string name;
		Stations* tmp_station1;
		Stations* tmp_station2;
		int num_station;
		int max_speed;

		Edges* tmp_edge;
		int len;
		std::vector<Edges>* tmp_line;

		for (int i = 0; i < 7; i++){               //genarate stations and line
			tmp_line = new std::vector<Edges>;
			std::cin >> line >> num_station >> len >> max_people >> max_speed;
			line_speed[line] = max_speed;
			line_max_people[line] = max_people;
			
			std::cin >> name;
			tmp_station2 = new Stations(line * 100, max_people, name);
			station.push_back(tmp_station2);
			for (int j = 1; j < num_station; j++) {   
				tmp_station1 = tmp_station2;
				std::cin >> name;
				tmp_station2 = new Stations(line * 100 + j, max_people, name);
				station.push_back(tmp_station2);
				tmp_edge = new Edges(len, tmp_station1, tmp_station2);
				tmp_station1->edge_push_back(tmp_edge);
				tmp_line->push_back(*tmp_edge);
				tmp_edge = new Edges(len, tmp_station2, tmp_station1);
				tmp_station2->edge_push_back(tmp_edge);
			}
			this->line.push_back(*tmp_line);

			Stations* tmp_station;
			tmp_line = new std::vector<Edges>;
			//std::reverse_copy(this->line[this->get_line_add(line)].begin(), this->line[this->get_line_add(line)].end(), tmp_line);
			for (auto i = this->line[this->get_line_add(line)].end(); i != this->line[this->get_line_add(line)].begin();) {
				i--;
				tmp_line->push_back(*i);
			}
			for (auto& i : *tmp_line) {             //reverse the line
				tmp_station = i.bgn;
				i.bgn = i.end;
				i.end = tmp_station;
			}
			this->line.push_back(*tmp_line);
		}

		for (auto i = station.begin(); i != station.end(); i++) {   //add transfer edge
			for (auto j = station.begin(); j != station.end(); j++) {
				if ((*i)->get_name() == (*j)->get_name() && (*i)->get_line() != (*j)->get_line()) {
					tmp_edge = new Edges(0, *i, *j);
					(*i)->edge_push_back(tmp_edge);
					transfer_station[(*i)->get_line()][(*j)->get_line()].push_back(*i);
				}
			}
		}
		
		for (int i = 0; i < 90; i++) {
			for (int j = 0; j < 90; j++) {
				this->mem_dest[i][j].clear();
			}
		}
	}
	~Map() {
		return;
	}

	Stations* get_station(int l, std::string n) {
		for (auto tmp = station.begin(); tmp != station.end(); tmp++) {
			if ((*tmp)->get_line() == l && n == (*tmp)->get_name()) return *tmp;
		}
		return NULL;
	}

	int get_line_add(int line) { //no pos or neg, just the differ between end station, just plus one.
		if (line > 4) {
			return (line - 2) * 2;
		}
		else {
			return (line - 1) * 2;
		}
	}

	Stations* get_line_end(Stations* fst, Stations* scd) {  //get the station which is in the end of line to ensure diraction
		bool tf = false;
		if (fst->get_line() != scd->get_line()) return NULL;
		auto tmp = this->line[this->get_line_add(fst->get_line())].begin();  //station's line transfer to vector's address to get line edge;
		if (tmp->bgn->get_no() == fst->get_no()) {
			tmp = this->line[this->get_line_add(fst->get_line())].end();
			tmp--;
			return tmp->end;
		}
		else if (tmp->bgn->get_no() == scd->get_no()) {
			return scd;
		}
		for (; tmp != this->line[this->get_line_add(fst->get_line())].end(); tmp++) {
			if (tmp->end->get_no() == fst->get_no()) {
				tf = true;
			}
			else if (tmp->end->get_no() == scd->get_no()) {
				if (tf) {
					tmp = this->line[this->get_line_add(fst->get_line())].end();
					tmp--;
					return tmp->end;
				}
				else {
					tmp = this->line[this->get_line_add(fst->get_line())].begin();
					return tmp->bgn;
				}
			}
			else continue;
		}
	}

	int len_btw_stations(Stations* bgn, Stations* end, Stations* line_end) {
		if (bgn->get_no() == end->get_no()) return 10;  //when end or start station is transfer station, this case occur;
		if (bgn->get_line() != end->get_line()) return -1;
		auto tmp1 = this->line[this->get_line_add(bgn->get_line())].begin();
		if (tmp1->bgn->get_no() == line_end->get_no()) {
			while (tmp1->bgn->get_no() != end->get_no()) tmp1++;
			auto tmp2 = this->line[this->get_line_add(bgn->get_line())].end();
			tmp2--;
			while (tmp2->end->get_no() != bgn->get_no()) tmp2--;
			return (tmp2 - tmp1 + 1) * tmp1->len;
		}
		else {
			while (tmp1->bgn->get_no() != bgn->get_no()) tmp1++;
			auto tmp2 = this->line[this->get_line_add(bgn->get_line())].end();
			tmp2--;
			while (tmp2->end->get_no() != end->get_no()) tmp2--;
			return (tmp2 - tmp1 + 1) * tmp1->len;
		}
	}

	int get_way_len(Stations* start_station, std::vector<dest> way) {   //caculate the lenth of way
		if (way.empty()) return 1 << 30; //way is none, return MAX_INT;
		int lenth = 0;
		auto tmp = way.begin();
		while (tmp != way.end()) {
			if (tmp->get_line() == 0) {
				start_station = tmp->get_dest_station();
				tmp++;
				continue;
			}
			lenth += len_btw_stations(start_station, tmp->get_dest_station(), this->get_line_end(start_station, tmp->get_dest_station()));
			start_station = tmp->get_dest_station();
			tmp++;
		}
		return lenth;
	}

	std::vector<dest> get_normal_way(Stations* bgn, Stations* end){
		if (!this->mem_dest[bgn->get_no()][end->get_no()].empty()) return this->mem_dest[bgn->get_no()][end->get_no()];

		int min_len = 1 << 30;
		int tmp_len;
		std::vector<dest> min_way;
		std::vector<dest> tmp_way;
		std::vector<Stations*> tmp_stations;
		std::vector<Stations*> bgn_stations, end_stations;
		
		if (!bgn->get_transfer_station().empty()){
			tmp_stations.clear();
			tmp_stations = bgn->get_transfer_station();
			bgn_stations.assign(tmp_stations.begin(), tmp_stations.end());
		}
		bgn_stations.push_back(bgn);

		if (!end->get_transfer_station().empty()) {
			tmp_stations.clear();
			tmp_stations = end->get_transfer_station();
			end_stations.assign(tmp_stations.begin(), tmp_stations.end());
		}
		end_stations.push_back(end);

		for (auto& i : bgn_stations) {
			for (auto& j : end_stations) {
				tmp_way.clear();
				tmp_way = get_normal_way_sub(i, j);
				tmp_len = get_way_len(i, tmp_way);
				if (tmp_len < min_len) {
					min_way.assign(tmp_way.begin(), tmp_way.end());
				}
			}
		}
		this->mem_dest[bgn->get_no()][end->get_no()] = min_way;
		return min_way;
	}

	std::vector<dest> get_normal_way_sub(Stations* bgn, Stations* end) { 
	// This function will search the way followed by minus transfer time for bots;
	// The line 2 connect to any other lines, which means max 2 times transferring between any stations;
	// Bots will check begin station is on the same line to the end station;
	// If not, he will find transfer station between two station and go though that station; 
	// It still not, he will find third line to transfer 2 times to get the end station;
		int min_len;
		int tmp_len;
		dest* tmp_dest;
		std::vector<dest> min_way;
		std::vector<dest> tmp_way;
		if (bgn->get_line() == end->get_line()) {   //two stations in one line
			tmp_dest = new dest(end->get_line(), end, this->get_line_end(bgn, end));
			min_way.push_back(*tmp_dest);
			return min_way;
		}
		else if (!this->transfer_station[bgn->get_line()][end->get_line()].empty()) {  // two stations in two lines, but there are transferring station
			min_len = 1 << 30;
			for (auto iter : this->transfer_station[bgn->get_line()][end->get_line()]) {
				tmp_way.clear();
				tmp_dest = new dest(iter->get_line(), &*iter, this->get_line_end(bgn, &*iter));
				tmp_way.push_back(*tmp_dest);
				tmp_dest = new dest(0, iter->get_transfer_station(end->get_line()), NULL); // end station is NULL because transferring
				tmp_way.push_back(*tmp_dest);
				tmp_dest = new dest(end->get_line(), end, this->get_line_end(iter->get_transfer_station(end->get_line()), end));
				tmp_way.push_back(*tmp_dest);
				tmp_len = get_way_len(bgn, tmp_way);
				if (tmp_len < min_len) {
					min_len = tmp_len;
					min_way.assign(tmp_way.begin(), tmp_way.end());
				}
			}
			return min_way;
		}
		else {  // two stations on two lines and there are no transferring station, using third line and transferring two times to get to the end station
			min_way.clear();
			min_len = 1 << 30;
			for (int transfer_line : {1, 2, 3, 4, 6, 7, 8}) {
				if (transfer_line == bgn->get_line() || transfer_line == end->get_line()) continue;
				for (Stations* fst_trans_station : this->transfer_station[bgn->get_line()][transfer_line]) {
					for (Stations* scd_trans_station : this->transfer_station[transfer_line][end->get_line()]) {
						tmp_dest = new dest(bgn->get_line(), fst_trans_station, this->get_line_end(bgn, fst_trans_station));
						tmp_way.push_back(*tmp_dest);
						tmp_dest = new dest(0, fst_trans_station->get_transfer_station(transfer_line), NULL);
						tmp_way.push_back(*tmp_dest);
						tmp_dest = new dest(transfer_line, scd_trans_station, this->get_line_end(fst_trans_station->get_transfer_station(transfer_line), scd_trans_station));
						tmp_way.push_back(*tmp_dest);
						tmp_dest = new dest(0, scd_trans_station->get_transfer_station(end->get_line()), NULL);
						tmp_way.push_back(*tmp_dest);
						tmp_dest = new dest(end->get_line(), end, this->get_line_end(scd_trans_station->get_transfer_station(end->get_line()), end));
						tmp_way.push_back(*tmp_dest);
						tmp_len = this->get_way_len(bgn, tmp_way);
						if (tmp_len < min_len) {
							min_way.assign(tmp_way.begin(), tmp_way.end());
							min_len = tmp_len;
						}
					}
				}
			}
			return min_way;
		}
	}

	bool generate_person(Stations* bgn = NULL, Stations* end = NULL, int num = 1) {
		if (this->now_time >= 60 * (23 - 6)) return false;
		People* tmp_people;
		for (int i = 0; i < num; i++) {
			if (bgn == NULL) bgn = this->station[rand() % this->station.size()];
			if (end == NULL) end = this->station[rand() % this->station.size()];
			tmp_people = new People(bgn, end, this->get_normal_way(bgn, end));
			this->person.push_back(tmp_people);
		}
		return true;
	}

	bool generate_metro(int line = 0, int num = 1) {
		Metros* tmp_metro;
		for (int i = 0; i < num; i++) {
			if (line == 0) {
				line = (rand() % 7) + 1;
				if (line >= 5) line++;
			}
			tmp_metro = new Metros(line, this->line_max_people[line], this->line_speed[line], this->line[this->get_line_add(line)]);
			this->metro.push_back(tmp_metro);
			tmp_metro = new Metros(line, this->line_max_people[line], this->line_speed[line], this->line[this->get_line_add(line) + 1]);
			this->metro.push_back(tmp_metro);
		}
		return true;
	}

	bool Generate_metro() {
		if (this->now_time > (23 - 6) * 60) return true;
		if (this->now_time % 4 == 0) this->generate_metro(1, 1);
		if (this->now_time % 4 == 0) this->generate_metro(2, 1);
		if (this->now_time % 5 == 0) this->generate_metro(3, 1);
		if (this->now_time % 4 == 0) this->generate_metro(4, 1);
		if (this->now_time % 6 == 0) this->generate_metro(6, 1);
		if (this->now_time % 5 == 0) this->generate_metro(7, 1);
		if (this->now_time % 7 == 0) this->generate_metro(8, 1);
		return true;
	}
	
	void nxt_scd(int core, int num_core);
	void nxt_scd_thread();

	void get_way_ans(Stations* bgn, Stations end, int bgn_time) {
		
	}

	std::vector<Stations*> get_station() {
		std::vector<Stations*> tmp_stations;
		for (auto iter = this->station.begin(); iter != this->station.end(); iter++)
			tmp_stations.push_back(*iter);
		return tmp_stations;
	}

private:
	int now_time;  //start by min
	int line_speed[9];
	int line_max_people[9];
	bool recode;
	std::vector<Point*> ans_way;
	Anwser* ans;
	std::vector<Stations*> station;
	std::vector<Stations*> transfer_station[9][9];
	std::list<Metros*> metro;
	std::list<People*> person;
	std::vector<std::vector<Edges>> line;
	std::map<int, std::map<int, std::vector<dest>>> mem_dest;
};