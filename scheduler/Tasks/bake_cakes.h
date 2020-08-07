#ifndef __BAKE_CAKES_H__
#define __BAKE_CAKES_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class BakeCakes : public ITask
{

public:
	BakeCakes(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline BakeCakes::BakeCakes(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void BakeCakes::run()
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

inline size_t BakeCakes::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool BakeCakes::isTaskDone() const
{
	return !m_cycles;
}

inline void BakeCakes::taskMessage() const
{
	std::cout << "Task: Bake Cakes, cycles left: " << m_cycles << std::endl;
}

#endif /*__BAKE_CAKES_H__*/
