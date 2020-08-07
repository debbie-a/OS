#ifndef __PLAY_MUSIC_H__
#define __PLAY_MUSIC_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class PlayMusic : public ITask
{

public:
	PlayMusic(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline PlayMusic::PlayMusic(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void PlayMusic::run()
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

inline size_t PlayMusic::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool PlayMusic::isTaskDone() const
{
	return !m_cycles;
}

inline void PlayMusic::taskMessage() const
{
	std::cout << "Task: Play Music, cycles left: " << m_cycles << std::endl;
}

#endif /*__EAT_CHOCOLATE_H__*/
