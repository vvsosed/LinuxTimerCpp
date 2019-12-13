
#pragma once

#include <functional>
#include <time.h>
#include <signal.h>

namespace ltmr {

class Timer {
public:
	using ClbkType = std::function<void()>;
	using TimeoutType = long long;

	Timer();

	~Timer();

	static bool init();

	bool once( TimeoutType _sec, ClbkType clbk );

	bool periodic( TimeoutType _sec, ClbkType clbk );

	bool stop();

	bool isCompleted() const;

	int overrun() const;

	inline operator bool() const {
		return m_isValid;
	}

private:
	static void timer_handler(int signo, siginfo_t *info, void *context);

	bool createAndArmTimer( const itimerspec& tmrSpec );

	void onTick();

private:
	unsigned long m_count = 0;
	ClbkType m_clbk;
	bool m_isValid;
	timer_t m_tid;
};

void printMonotonicTimerInfo();

} // namespace ltmr
