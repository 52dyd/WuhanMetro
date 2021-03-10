#pragma once
#include<iostream>

class Stations;

class Edges
{
public:
	Edges(int len, Stations* bgn, Stations* end){
		this->len = len;
		this->bgn = bgn;
		this->end = end;
	}
	~Edges(){}
	int len;	//len == 0  transfer station							
	Stations* bgn;
	Stations* end;
};