#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <windows.h>


class TickTimer : public EngineObjectBase
{
private:
protected:
	unsigned int started_at_time = 0;
	unsigned int ticks_to_wait = 0;

public:

	TickTimer() 
		: started_at_time(uninit::UINT)
		, ticks_to_wait(uninit::UINT)
	{}

	TickTimer(unsigned int time_length)
		: started_at_time(uninit::UINT)
		, ticks_to_wait(time_length)
	{}

	void set_time_length(unsigned int ms)
	{
		ticks_to_wait = ms;
	}

	void reset()
	{
		ticks_to_wait = 0;
		started_at_time = 0;
	}

	void start()
	{
		// todo: throw error if no wait time has been set

		started_at_time = SDL_GetTicks();
	}

	bool time_elapsed() const
	{
		auto current_time = SDL_GetTicks();
		return (current_time - started_at_time) > ticks_to_wait;
	}

};



class CallbackTimer : public TickTimer
{
public:
	typedef std::function<void()> CallbackVoidType;

private:
protected:
	CallbackVoidType cb;	
	bool callback_sent = false;
public:

	void set_callback(CallbackVoidType callback)
	{
		cb = callback;
	}

	void update()
	{
		if (time_elapsed() && !callback_sent)
		{
			callback_sent = true;
			cb();
		}
	}
};

#endif // !TIMER_H
