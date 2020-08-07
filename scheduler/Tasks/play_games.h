#ifndef __PLAY_GAMES_H__
#define __PLAY_GAMES_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class PlayGames : public ITask
{

public:
	PlayGames(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline PlayGames::PlayGames(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void PlayGames::run()
{
	taskMessage();
	m_cycles--;

	if (isTaskDone())
	{
		m_time = 0;
	}
	else
    	{
		m_time = Time::now() + m_interval;
    	}

}

inline size_t PlayGames::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool PlayGames::isTaskDone() const
{
	return !m_cycles;
}

inline void PlayGames::taskMessage() const
{
	std::cout << "Task: Play Games, cycles left: " << m_cycles << std::endl;
}

#endif /*__PLAY_GAMES_H__*/
