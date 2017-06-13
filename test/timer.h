#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <unistd.h>

class Timer	{
public:
	Timer() { Reset(); }
	void Reset() { gettimeofday(&this->time_start_, &time_zone_); }
	//Returns time in seconds.
	double Elapsed() {
		struct timeval time_end;
		gettimeofday(&time_end, &time_zone_);
		double t1, t2;
		t1 =  (double)time_start_.tv_sec +
				(double)time_start_.tv_usec/(1000*1000);
		t2 =  (double)time_end.tv_sec + (double)time_end.tv_usec/(1000*1000);
		return t2-t1;
	}

private:
	struct timeval time_start_;
	struct timezone time_zone_;
};

#endif
