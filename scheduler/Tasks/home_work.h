#ifndef __HOME_WORK_H__
#define __HOME_WORK_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class HomeWork : public ITask
{

public:
	HomeWork(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline HomeWork::HomeWork(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void HomeWork::run()
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

inline size_t HomeWork::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool HomeWork::isTaskDone() const
{
	return !m_cycles;
}

inline void HomeWork::taskMessage() const
{
	std::cout << "Task: Home Work, cycles left: " << m_cycles << std::endl;
}

#endif /*__HOME_WORK_H__*/
