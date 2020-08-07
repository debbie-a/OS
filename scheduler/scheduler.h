#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <queue>
#include "Tasks/itask.h"
#include "time_.h"
#include "shared_pointer.h"


class Scheduler
{
public:
	void run();
	void addTask(SharedPtr<ITask>& newTask);

private:
    	class Compare
    	{
    	public:
        	bool operator()(SharedPtr<ITask>& task1, SharedPtr<ITask>& task2)
        	{
            		return task1->getNextRunPeriod() > task2->getNextRunPeriod();
        	}
    	};

    	bool shouldRun() const;
    	void handleNextTask(SharedPtr<ITask>&);
	
    	std::priority_queue <SharedPtr<ITask>, std::vector<SharedPtr<ITask> >, Compare> m_queueTask;
};


inline void Scheduler::addTask(SharedPtr<ITask>& newTask)
{
	m_queueTask.push(newTask);
}


#endif /*__SCHEDULER_H__*/
