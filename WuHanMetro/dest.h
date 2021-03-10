#pragma once
#include"station.h"

class dest
{
public:
	dest(int line, Stations* dest, Stations* line_end) {
		this->line = line;
		this->dest_station = dest;
		this->line_end = line_end;
	}
	~dest() {}

	int get_line(void) {
		return this->line;
	}

	Stations* get_dest_station(void) {
		return this->dest_station;
	}

	Stations* get_line_end(void) {
		return this->line_end;
	}

private:
	int line; //It's transferring when line is 0;
	Stations* dest_station;
	Stations* line_end;
};