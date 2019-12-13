#include "tmrwrap.h"

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <limits>

namespace ltmr {

void Timer::timer_handler(int signo, siginfo_t *info, void *context) {
	if (info->si_code == SI_TIMER) {
		static_cast<Timer*>(info->si_value.sival_ptr)->onTick();
	}
}

Timer::Timer() {
	sigevent sigev     = {};
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo  = SIGRTMIN;
	sigev.sigev_value.sival_ptr = this;
	m_isValid = (-1) != timer_create(CLOCK_MONOTONIC, &sigev, &m_tid);
	std::cout << "create instance" << std::endl;
}

Timer::~Timer() {
	if ( !isCompleted() ) {
		stop();
	}

	if ( m_isValid ) {
		timer_delete(&m_tid);
	}

	m_isValid = false;
	std::cout << "delete instance" << std::endl;
}

bool Timer::init() {
	sigset_t mask;
	sigemptyset(&mask);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = Timer::timer_handler;
	if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
		perror("sigaction failed");
		return false;
	}

	return true;
}

bool Timer::once( TimeoutType _sec, ClbkType clbk ) {
	if ( !m_isValid ) {
		return false;
	}

	m_clbk = clbk;
	m_count = 1;
	itimerspec ival = {};
	ival.it_value.tv_sec = _sec;

	return createAndArmTimer(ival);
}

bool Timer::periodic( TimeoutType _sec, ClbkType clbk ) {
	if ( !m_isValid ) {
		return false;
	}

	m_clbk = clbk;
	m_count = std::numeric_limits<decltype(m_count)>::max();
	itimerspec ival = {};
	ival.it_value.tv_sec = _sec;
	ival.it_interval.tv_sec = _sec;
	return createAndArmTimer(ival);
}

bool Timer::stop() {
	if ( !m_isValid ) {
		return false;
	}

	m_count = 0;
	return (-1) != timer_settime(m_tid, 0, NULL, NULL);
}

bool Timer::isCompleted() const {
	return 0 == m_count;
}

int Timer::overrun() const {
	return m_isValid ? timer_getoverrun(m_tid) : 0;
}

bool Timer::createAndArmTimer( const itimerspec& tmrSpec ) {
	return (-1) != timer_settime(m_tid, 0, &tmrSpec, NULL);
}

void Timer::onTick() {
	if ( m_clbk ) {
		m_clbk();
	}

	if ( m_count == 0 ) {
		stop();
		m_clbk = nullptr;
	}
	else {
		--m_count;
	}
}

void printMonotonicTimerInfo() {
	struct timespec ts, tm;
	clock_getres(CLOCK_MONOTONIC, &ts);
	clock_gettime(CLOCK_MONOTONIC, &tm);
	std::cout << "CLOCK_MONOTONIC res: [" << ts.tv_sec << "]sec [" << ts.tv_nsec << "]nsec" << std::endl;
	std::cout << "system up time: [" << tm.tv_sec << "]sec [" << tm.tv_nsec << "]nsec" << std::endl;
}

} // namespace ltmr
