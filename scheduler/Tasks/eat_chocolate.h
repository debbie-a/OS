#ifndef __EAT_CHOCOLATE_H__
#define __EAT_CHOCOLATE_H__

#include <unistd.h>
#include <iostream>
#include "itask.h"
#include "../time_.h"


class EatChocolate : public ITask
{

public:
	EatChocolate(size_t startIn, size_t cycles, size_t interval);

	/*virtual*/ void run();
	/*virtual*/ size_t getNextRunPeriod() const;

private:
    	bool isTaskDone() const;
	void taskMessage() const;

    	Time m_time;
	size_t m_cycles;
	size_t m_interval;
};


inline EatChocolate::EatChocolate(size_t startIn, size_t cycles, size_t interval):m_time(Time::now() + startIn), m_cycles(cycles),m_interval(interval)
{
}

inline void EatChocolate::run()
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

inline size_t EatChocolate::getNextRunPeriod() const
{
	return m_time.getMilliseconds();
}

inline bool EatChocolate::isTaskDone() const
{
	return !m_cycles;
}

inline void EatChocolate::taskMessage() const
{
	std::cout << "Task: Eat Chocolate, cycles left: " << m_cycles << std::endl;
}
#endif /*__EAT_CHOCOLATE_H__*/
