# Без синхронизации

## Реализация

```c++
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <chrono>

using namespace std;

deque<int> list_s;
deque<int> list_r;

bool run = 1;

const int SLEEPTIME = 1000;

void function_a() {
	int i = 0;
	while (run) {
		i++;
		list_s.push_back(i);
		this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		if (i > 10) {
			run = 0;
		}
	}
}

void function_b() {
	while (run) {
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			list_r.push_back(s * s);
		}
		else {
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

void function_c() {
	while (run) {
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			list_r.push_back(s / 3);
		}
		else {
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

void function_d() {
	while (run) {
		if (list_r.size()) {
			int r = list_r.back();
			list_r.pop_back();
			cout << r << "\n";
		}
		else {
			cout << "List R is empty\n";
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

int main() {
	thread a(function_a);
	thread b(function_b);
	thread c(function_c);
	thread d(function_d);
}
```

## Результат

Программа падает, когда из deque читается значение

# Синхронизация чтения

## Реализация

```c++
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <chrono>
#include <mutex>

using namespace std;

deque<int> list_s;
deque<int> list_r;

bool run = 1;

const int SLEEPTIME = 1000;

mutex s_mutex;
mutex r_mutex;

void function_a() {
	int i = 0;
	while (run) {
		i++;
		s_mutex.lock();
		list_s.push_back(i);
		s_mutex.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		if (i > 10) {
			run = 0;
		}
	}
}

void function_b() {
	while (run) {
		s_mutex.lock();
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			s_mutex.unlock();

			r_mutex.lock();
			list_r.push_back(s * s);
			r_mutex.unlock();
		}
		else {
			s_mutex.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

void function_c() {
	while (run) {
		s_mutex.lock();
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			s_mutex.unlock();

			r_mutex.lock();
			list_r.push_back(s / 3);
			r_mutex.unlock();
		}
		else {
			s_mutex.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

void function_d() {
	while (run) {
		r_mutex.lock();
		if (list_r.size()) {
			int r = list_r.back();
			list_r.pop_back();
			r_mutex.unlock();
			cout << r << "\n";
		}
		else {
			r_mutex.unlock();
			cout << "List R is empty\n";
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

int main() {
	thread a(function_a);
	thread b(function_b);
	thread c(function_c);
	thread d(function_d);

	a.join();
	b.join();
	c.join();
	d.join();
}
```

## Результат

Программа работает стабильно, но при каждом запуске результат разный (как и ожидалось)

# Взаимоблокировка вследствие состязания

```c++
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <chrono>
#include <mutex>

using namespace std;

deque<int> list_s;
deque<int> list_r;

bool run = 1;

const int SLEEPTIME = 100;

mutex s_mutex;
mutex r_mutex;

void function_a() {
	int i = 0;
	while (run) {
		i++;
		s_mutex.lock();
		list_s.push_back(i);
		s_mutex.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		if (i > 10) {
			run = 0;
		}
	}
}

void function_b() {
	while (run) {
		cout << "enter b\n";
		s_mutex.lock();
		this_thread::sleep_for(std::chrono::milliseconds(10));
		r_mutex.lock();
		
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();	
			list_r.push_back(s * s);

			
		}
		else {
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}

		cout << "exit b\n";
		r_mutex.unlock();
		s_mutex.unlock();
	}
}

void function_c() {
	while (run) {
		cout << "enter c\n";
		r_mutex.lock();
		s_mutex.lock();
		

		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			list_r.push_back(s / 3);
			
			
		}
		else {
			
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}

		cout << "exit c\n";
		s_mutex.unlock();
		r_mutex.unlock();
	}
}

void function_d() {
	while (run) {
		r_mutex.lock();
		if (list_r.size()) {
			int r = list_r.back();
			list_r.pop_back();
			r_mutex.unlock();
			cout << r << "\n";
		}
		else {
			r_mutex.unlock();
			cout << "List R is empty\n";
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

int main() {
	thread a(function_a);
	thread b(function_b);
	thread c(function_c);
	thread d(function_d);

	a.join();
	b.join();
	c.join();
	d.join();
}
```

## Результат

Происхоит взаимоблокировка потоков b и c, дальнейшие операции не выполняются.

# Deadlock

```c++
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <chrono>
#include <cstdlib> 
#include <mutex>
#include <cassert>

using namespace std;

deque<int> list_s;
deque<int> list_r;

bool run = 1;

const int SLEEPTIME = 100;

mutex s_mutex;
mutex r_mutex;

void function_a() {
	int i = 0;
	while (run) {
		i++;
		s_mutex.lock();
		list_s.push_back(i);
		s_mutex.unlock();

		this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		if (i > 10) {
			run = 0;
		}
	}
}

void function_b() {
	while (run) {
		s_mutex.lock();
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			s_mutex.unlock();

			r_mutex.lock();

			if (rand() % 5 == 0) {
				deque<int> a;
				a[0];
			}

			list_r.push_back(s * s);
			r_mutex.unlock();
		}
		else {
			s_mutex.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

void function_c() {
	while (run) {
		s_mutex.lock();
		if (list_s.size()) {
			int s = list_s.back();
			list_s.pop_back();
			s_mutex.unlock();

			r_mutex.lock();
			list_r.push_back(s / 3);
			r_mutex.unlock();
		}
		else {
			s_mutex.unlock();
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

void function_d() {
	while (run) {
		r_mutex.lock();
		if (list_r.size()) {
			int r = list_r.back();
			list_r.pop_back();
			r_mutex.unlock();
			cout << r << "\n";
		}
		else {
			r_mutex.unlock();
			cout << "List R is empty\n";
			this_thread::sleep_for(std::chrono::milliseconds(SLEEPTIME));
		}
	}
}

int main() {
	thread a(function_a);
	thread b(function_b);
	thread c(function_c);
	thread d(function_d);

	a.join();
	b.join();
	c.join();
	d.join();
}
```

## Результат

При достижении ошибки в консоль ничего не выводится - программа не может выполнить функцию ни в одном потоке