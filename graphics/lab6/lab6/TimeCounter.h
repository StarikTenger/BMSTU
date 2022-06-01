#pragma once
#include <cstdlib>
#include <sys/timeb.h>
#include <vector>

int get_milli_count();

class TimeCounter {
private:
	int frames = 0;
	int ms_sum = 0;
	std::vector<char> values;
	// Temp
	int time_marked = 0;
	double standart_deviation;
	int frame_limit = 1000;
public:
	void start_frame();
	void end_frame();
	int get_frames();
	int get_ms_sum();
	double get_average_time();
	double get_standart_deviation();
	bool check_frame_limit();
};

