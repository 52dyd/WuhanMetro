#pragma once
#include<vector>
#include<utility>
#include"metro.h"
#include"dest.h"

class Stations;
class dest;

class People
{
private:
	int mode; //1 is waiting in station, 2 is staying in metro, 3 is transferring between lines; 
	int transfer_time;
	Stations* now_station;
	Metros* now_metro;
	Stations* bgn, * end;
	int init_time;              //waiting for coding..../////////////////<<<<<<<<<<<<<<<<<<<----------------------
	std::vector<dest> way;
	std::vector<dest>::iterator now;
	bool live;

public:
	People(Stations* bgn, Stations* end, std::vector<dest> way) {
		this->now_metro = NULL;
		this->bgn = bgn;
		this->end = end;
		this->way.swap(way);
		if (this->bgn->get_person(this)) {
			this->mode = 1;
			this->now_station = this->bgn;
			this->now = this->way.begin();
		}
		this->live = true;
	}
	~People() {}

	bool tf_live() {
		return this->live;
	}

	int nxt_scd(void) {  //0 for success, -1 for error, 1 for end of this gay :)
		if (mode == 1) {  //waiting in station
			auto tmp = this->now_station->has_metro(now->get_line(), now->get_line_end());
			if (tmp != NULL) { // has metro which is gay wanted, get on !
				this->now_station->out_person(this);
				this->now_metro->get_person(this);
				this->mode = 2;
				this->now_station = NULL;
			}
		}
		else if (mode == 2) {  // by metro, maybe stop in station ?
			if (!now_metro->station_on_line(now->get_dest_station())) return -1;  //over destination ... fuck!
			auto tmp = now_metro->in_station();
			if (tmp == now->get_dest_station()) {   //if this gay are in his destination, his two choises: end of life or transfering
				now_metro->out_person(this);
				now++;
				if (now == way.end() && tmp->get_no() == this->end->get_no()) {
					this->live = false;
					return 1; //end of life
				}
				else if (now->get_line() == 0) { //to transfer
					if (now->get_dest_station()->get_name() != now_station->get_name()) return -1; //way error, transfer station not match!
					if (tmp->get_person(this)) {
						this->now_station = tmp;
						this->mode = 3;
						this->transfer_time = this->now_station->transferring_time();
						this->now_metro = NULL;
					}
					else return -1; //error to get person in station
				}
				else return -1; //after getting out the metro, don't leave station or transfer to another line, the way must be error;
			}
		}
		else if (mode == 3) { // transferring;
			if (this->transfer_time > 1) {
				this->transfer_time--;
			}
			else {
				this->transfer_time = 0;
				if (!this->now_station->out_person(this)) return -1; // out of station failed
				auto tmp = this->now->get_dest_station();
				if (!tmp->get_person(this)) return -1; //get in station failed
				now_station = tmp;
				now++; //then, this gay has one choice, waiting metro on another line;	
				this->mode = 1;
			}
		}
		else return -1;
		return 0;
	}
};