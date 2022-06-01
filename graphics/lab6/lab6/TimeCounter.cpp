#include "TimeCounter.h"

int get_milli_count() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

void TimeCounter::start_frame() {
	time_marked = get_milli_count();
}

void TimeCounter::end_frame() {
	auto frame = get_milli_count() - time_marked;
	ms_sum += frame;
	values.push_back(frame);
	frames++;
}

int TimeCounter::get_frames() {
	return frames;
}

int TimeCounter::get_ms_sum() {
	return ms_sum;
}

double TimeCounter::get_average_time() {
	return ms_sum * 1.0 / frames;
}

double TimeCounter::get_standart_deviation() {
	double sum = 0.0;
	double average = get_average_time();
	for (auto& v : values) {
		sum += (average - v) * (average - v);
	}
	return sqrt(sum / frames);
}

bool TimeCounter::check_frame_limit() {
	return frames >= frame_limit;
}
