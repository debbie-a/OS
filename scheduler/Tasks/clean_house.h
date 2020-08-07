#ifndef __CLEAN_HOUSE_H__
#define __CLEAN_HOUSE_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class CleanHouse : public ITask
{

public:
	CleanHouse(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline CleanHouse::CleanHouse(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void CleanHouse::run()
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

inline size_t CleanHouse::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool CleanHouse::isTaskDone() const
{
	return !m_cycles;
}

inline void CleanHouse::taskMessage() const
{
	std::cout << "Task: Clean House, cycles left: " << m_cycles << std::endl;
}

#endif /*__CLEAN_HOUSE_H__*/
