#include "tmrwrap.h"

#include <iostream>
#include <unistd.h>

void clbk1() {
	std::cout << "run clbk1" << std::endl;
}

struct SomeClass {
	SomeClass( ltmr::Timer::TimeoutType timeout_sec ) {
		m_timer.periodic(timeout_sec, [this](){ print(); });
	}

	void print() {
		std::cout << "run SomeClass: " << m_counter << std::endl;
		//if (0 == --m_counter) {
			m_timer.stop();
		//}
	}

	bool isCompleted() const {
		return m_timer.isCompleted();
	}

	int m_counter = 13;
	ltmr::Timer m_timer;
};

int main(int argc, char**argv) {
	if ( !ltmr::Timer::init() ) {
		return -1;
	}

	std::cout << "Simple c++ wrapper around POSIX timers!" << std::endl;
	ltmr::printMonotonicTimerInfo();

	ltmr::Timer timer1;
	if (timer1) {
		timer1.once(5, clbk1);
	}

	SomeClass sclass(1);

	while( !timer1.isCompleted() || !sclass.isCompleted() ) {
		std::cout << "sleep" << std::endl;
		sleep(3);
	}

	std::cout << "completed" << std::endl;
	return 0;
}

