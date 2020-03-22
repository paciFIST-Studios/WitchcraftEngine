#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <windows.h>


class cTimer
{
private:
protected:

public:
	static unsigned int constexpr UNINIT_ID = 0;
	static DWORD constexpr UNINIT_START_TIME = 0;
	static DWORD constexpr UNINIT_INTERVAL_LEN = 0;
	static bool constexpr UNINIT_TIME_ELAPSED = true;

	unsigned int _id;
	DWORD _start_time;			// ms (miliseconds)
	DWORD _interval_length;		// ms
	bool _time_period_elapsed;

	cTimer()
		: _id(UNINIT_ID)
		, _start_time(UNINIT_START_TIME)
		, _interval_length(UNINIT_INTERVAL_LEN)
		, _time_period_elapsed(UNINIT_TIME_ELAPSED)
	{}

	void start()
	{
		_start_time = timeGetTime();
		_time_period_elapsed = false;
	}

	void update()
	{
		if (_time_period_elapsed)
			return;

		DWORD elapsed_time = timeGetTime() - _start_time;

		if (elapsed_time > _interval_length)
		{
			_time_period_elapsed = true;
		}
	}
};

#endif // !TIMER_H
