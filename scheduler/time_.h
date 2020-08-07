#ifndef __TIME_H__
#define __TIME_H__

#include <iostream>


class Time
{
public:
	Time(size_t milliseconds);

    	static Time now();
    	size_t getMilliseconds() const;

private:
    	size_t m_milliseconds;
};


bool operator==(const Time& time1, const Time& time2);
Time operator+(const Time& time1, const Time& time2);


inline Time::Time(size_t milliseconds):m_milliseconds(milliseconds)
{
}

inline Time Time::now()
{
	size_t now = static_cast<size_t>(time(nullptr));

    	return now;
}

inline size_t Time::getMilliseconds() const
{
    	return m_milliseconds;
}

inline bool operator==(const Time& time1, const Time& time2)
{
	return time1.getMilliseconds() ==  time2.getMilliseconds();
}

inline Time operator+(const Time& time1, const Time& time2)
{
    	return time1.getMilliseconds() + time2.getMilliseconds();
}

#endif /*__TIME_H__*/
