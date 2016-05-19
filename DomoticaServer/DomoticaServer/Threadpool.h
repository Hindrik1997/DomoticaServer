#pragma once

#include <mutex>
#include <deque>
#include <thread>
#include <memory>
#include <vector>
#include <condition_variable>

#include "Worker.h"

//Task interface base class
class ITask;

//Internal class so i can use references for safer usage of the pool. Since references cannot be stored in a queue.
class ITaskWrapper
{
public:
	ITaskWrapper(ITask& task) : m_Task(task)
	{
	}	
	ITask& m_Task;	
};




class Threadpool {
public:
	Threadpool(size_t threads);
	//Do not pass a temporary object in here!
	void Enqueue(ITask& job);
	~Threadpool();
	//Clears the deque of tasks
	void EmptyTasks();
	//Clears the ram used by the deque, but only if empty
	void ClearDequeRam();
	
	//Gets the size of all tasks in the pool.
	inline int GetSize();
private:
	std::deque<ITaskWrapper> m_Tasks;
	std::vector<std::thread> m_Workers;
	std::condition_variable m_Cond;
	std::mutex m_Queue_Mutex;
	bool m_Stop;
	friend class Worker;
};

inline int Threadpool::GetSize() { return (int)m_Tasks.size();  }

