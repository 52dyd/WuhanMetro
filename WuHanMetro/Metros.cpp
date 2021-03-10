#include"station.h"

int Metros::nxt_scd() {
	if (this->tf_station) {
		if (this->now_station->out_metro(this)) {
			this->tf_station = false;
			this->countdown = (*now).len / this->speed;
		}
		else return -1; //error in getting outstation
	}
	else if (this->countdown > 1) {
		this->countdown--;
	}
	else if (this->countdown == 1) {
		if (this->now_station->get_metro(this)) {
			this->tf_station = 1;
			this->now++;
			if (now != this->way.end()) {
				this->now_station = this->now->bgn;
				this->nxt_station = this->now->end;
				this->countdown = 0;
			}
			else {
				this->live = false;
				return 2;
			}
		}
		else return -1;
	}
	else {
		return -1;
	}
	return 0;
}