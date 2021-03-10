#pragma once
#include<iostream>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
class Point;

class Edgep  
//edge_plus
{
public:
	Edgep(int len, float crowded_rate, Point* end, int time) {
		this->len = len;
		this->crowded_rate = crowded_rate;
		this->end = end;
		this->time = time;
	}
	~Edgep() {}

	Point* end;
	int time;
	int len;
	float crowded_rate;
};

class Point
{
public:
	Point(int time, Stations* station) {
		this->time = time;
		this->station = station;
	}
	
	~Point() {}
	
	bool add_edgep(Edgep* edgep) {
		if (!this->station->tf_nxt_station(edgep->end->station)) return false;
		this->nxt_point.push_back(*edgep);
	}
	
	bool in_line;
	Stations* station;
	int time;
	float min_weight; //minus weight until now, remember maxxing before using.
	Point* relaxation_point;
	std::vector<Edgep> nxt_point; //Edges has len member, which could be length between station and crowded rate of line(station);
};


class Anwser
{
public:
	Anwser(int start_time) { this->start_time = start_time; }
	~Anwser() {}

	bool add_point_edgep(Stations* bgn_station, int bgn_time, Stations* end_station, int end_time, float crowded_rate, int len) {
		Edgep* tmp_edgep;
		tmp_edgep = new Edgep(len, crowded_rate, this->get_point_add(end_station, end_time), end_time - bgn_time);
		this->get_point_add(bgn_station, bgn_time)->add_edgep(tmp_edgep);
		return true;
	}
	
	Point* get_point_add(Stations* station, int time) {
		for (auto& i : point) {
			if (i.station == station && i.time == time) return &i;
		}
		auto tmp_point = new Point(time, station);
		return tmp_point;
	}

	Point* get_exist_point_add(Stations* station, int time) {
		for (auto& i : point) {
			if (i.station == station && i.time == time) return &i;
		}
		return NULL;
	}

	float get_weight(Edgep* edgep, int mode, float mode_3_rate) { //mode 1 is crowded rate, 0 is len
		if (mode == 1) return edgep->crowded_rate * edgep->time;
		else if (mode == 2) return 0;
		else if (mode == 0) return (float)edgep->len;
		else if (mode == 3) {
			if (edgep->crowded_rate > mode_3_rate) return (1 << 30) + (1 << 29);
			else return edgep->crowded_rate * edgep->time;
		}
	}

	/*********************************************************************************************
	THIS IS SOMETHING ERROR, TRANSFER STAIONS NOW_PEOPLE IS TWO OR THREE STATIONS SUM, WHICH MEANS IT NEEDS TO KEEP SAME
	WAITING TO DEBUG...
	*********************************************************************************************/

	/*********************************************************************************************
	WHEN STATION AND METRO ARE FULL, THERE WON'T GETIN OR GETOUT, IT'S A BUG...
	*********************************************************************************************/

	std::vector<Edgep> spfa(Point* start_point, Stations* end_station, int mode, float mode_3_rate) {  //mode 1 is crowded rate, mode 0 is len, mode 2 is time, mode 3 is crowded line is broken
		Point* tmp_point;
		Edgep* tmp_edegp;
		std::vector<Edgep> ans;
		std::queue<Point*> point_waited;
		for(auto& i:this->point){
			i.min_weight = 1 << 30;
			i.relaxation_point = NULL;
		}
		point_waited.push(start_point);
		start_point->in_line = true;
		while (!point_waited.empty()) {
			tmp_point = point_waited.front();
			point_waited.pop();
			for (auto& i : tmp_point->nxt_point) {
				if (i.end->min_weight > tmp_point->min_weight + this->get_weight(&i, mode, mode_3_rate)) { //mode3 rate is max crowded rate 
					i.end->min_weight = tmp_point->min_weight + this->get_weight(&i, mode, mode_3_rate);  //SPFA REFRASH
					i.end->relaxation_point = tmp_point;
					for (auto tmp : i.end->nxt_point) {
						if (tmp.end->in_line) continue;
						point_waited.push(tmp.end);
					}
				}
			}
			tmp_point->in_line = false;
		}
		
		if (mode == 2) {
			int i = 1;
			ans.clear();
			for (tmp_point = this->get_exist_point_add(end_station, this->start_time); tmp_point == NULL; tmp_point = this->get_exist_point_add(end_station, this->start_time + i), i++) {
				if (tmp_point->min_weight < 1 << 30) break;
			}
			if (tmp_point == NULL) return ans;  //CAN'T ARRIVE IN TIME RANGE
			while (tmp_point != start_point) {
				for (auto i = tmp_point->relaxation_point->nxt_point.begin(); i != tmp_point->relaxation_point->nxt_point.end(); i++) {
					if (i->end != tmp_point) continue;
					else {
						tmp_edegp = &*i;
						goto nxt;
					}
				}
				ans.clear(); //?
				return ans;  //maybe no need
			nxt:ans.push_back(*tmp_edegp);
				tmp_point = tmp_point->relaxation_point;
			}
			std::reverse(ans.begin(), ans.end());
			return ans;
		}
		else if (mode == 1 || mode == 0 || mode == 3) {
			ans.clear();
			int i;
			float min_weight = 1 << 30;
			Point* min_point;
			for (tmp_point = this->get_exist_point_add(end_station, start_time), i = 1; tmp_point != NULL; i++) {
				if (tmp_point->min_weight < min_weight) {
					min_point = tmp_point;
					min_weight = tmp_point->min_weight;
				}
				tmp_point = this->get_exist_point_add(end_station, start_time + i);
			}
			if (min_weight == 1 << 30) return ans; //CAN'T arrive
			while (tmp_point != start_point) {
				for (auto i = tmp_point->relaxation_point->nxt_point.begin(); i != tmp_point->relaxation_point->nxt_point.end(); i++) {
					if (i->end != tmp_point) continue;
					else {
						tmp_edegp = &*i;
						goto nxt1;
					}
				}
				ans.clear();
				return ans;
			nxt1:ans.push_back(*tmp_edegp);
				tmp_point = tmp_point->relaxation_point;
			}
			std::reverse(ans.begin(), ans.end());
			return ans;
		}
		else return ans;
	}

private:
	int start_time;
	std::vector<Point> point;
};
