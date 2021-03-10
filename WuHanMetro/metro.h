#pragma once
#include<list>
#include<vector>

class Stations;
class People;

class Metros
{
public:
	Metros(int line, int max_people, int speed, std::vector<Edges> way) {
		this->line = line;
		this->speed = speed;
		this->now_people = 0;
		this->max_people = max_people;
		this->tf_station = 1;
		this->now_people = 1;
		this->way = way;
		this->now = this->way.begin();
		this->now_station = now->bgn;
		this->end_station = (--(this->way.end()))->end;
		this->live = true;
	}
	~Metros() {}

	bool station_on_line(Stations* s) {		//检测站点是否在路线内
		for (auto tmp = now; tmp != way.end(); tmp++) { //从当前站点一直到终点站
			if (tmp->end == s) return true;
		}
		return false;
	}

	Stations* in_station() {  //是否在车站
		if (this->tf_station) {
			return now_station;
		}
		return NULL;
	}

	bool get_person(People* p) {
		if (this->now_people == this->max_people) return false;
		people.push_back(p);
		this->now_people++;
		return true;
	}

	bool out_person(People* p) {
		for (auto tmp = people.begin(); tmp != people.end(); tmp++) {
			if ((*tmp) == p) {
				tmp = people.erase(tmp);
				return true;
			}
		}
		return false;
	}
	
	int get_line() {           
		return this->line;
	}
	
	float get_crowd_rate(void) {
		return (float)this->now_people / (float)this->max_people;
	}

	int get_countdown() {
		return this->countdown;
	}

	Stations* get_end_station(void) {
		return this->end_station;
	}

	bool get_live() {
		return this->live;
	}

	int nxt_scd();

private:
	int line;
	int now_people;
	int max_people;
	int countdown;
	int speed;
	bool tf_station;  //1 is in, 0 is out
	bool live;
	Stations* now_station;
	Stations* nxt_station;
	Stations* end_station;
	std::list<People*> people;
	std::vector<Edges> way;
	std::vector<Edges>::iterator now;
};