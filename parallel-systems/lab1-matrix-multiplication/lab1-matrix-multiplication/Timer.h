#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <deque>
#include <string>
#include <sys/timeb.h>

int get_milli_count() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

class Timer {
private:
	std::deque<std::pair<int, std::string>> records;

public:
	Timer() {}

	void start(std::string record_name) {
		records.push_back({ get_milli_count(), record_name });
	}

	int finish() {
		int time_now = get_milli_count();
		if (records.empty()) {
			std::cerr << "Unable to finish: timer is not set";
			return 0;
		}
		int delta_time = time_now - records.back().first;
		std::cout << records.back().second << " : " << delta_time << "ms\n";

		auto time_error = get_milli_count() - time_now;
		for (auto& record : records) {
			record.first -= time_error;
		}
	}
};