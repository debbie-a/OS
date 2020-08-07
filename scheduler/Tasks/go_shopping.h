#ifndef __GO_SHOPPING_H__
#define __GO_SHOPPING_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class GoShopping : public ITask
{

public:
	GoShopping(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline GoShopping::GoShopping(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void GoShopping::run()
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

inline size_t GoShopping::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool GoShopping::isTaskDone() const
{
	return !m_cycles;
}

inline void GoShopping::taskMessage() const
{
	std::cout << "Task: Go Shopping, cycles left: " << m_cycles << std::endl;
}

#endif /*__GO_SHOPPING_H__*/
