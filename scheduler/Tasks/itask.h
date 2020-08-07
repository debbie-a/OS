#ifndef __ITASK_H__
#define __ITASK_H__


struct ITask
{
    	virtual ~ITask();
	virtual void run() = 0;
	virtual size_t getNextRunPeriod() const = 0;

};

inline ITask::~ITask()
{
}

#endif/*__ITASK_H__*/
