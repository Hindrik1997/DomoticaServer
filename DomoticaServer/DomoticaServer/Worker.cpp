#include "Worker.h"
#include "ITask.h"
#include "Threadpool.h"

void Worker::operator()()
{
	ITask* task;
	while (true)
	{
		//Mutex om problemen met de queue te voorkomen
		std::unique_lock<std::mutex> locker(m_Pool.m_Queue_Mutex);
		//std::condition om busy-waiting te voorkomen.
		m_Pool.m_Cond.wait(locker, [this]() -> bool { return !m_Pool.m_Tasks.empty() || m_Pool.m_Stop; });
		if (m_Pool.m_Stop)
			return;
		if (!m_Pool.m_Tasks.empty())
		{
			//Task van pool halen en uitvoeren
			task = &(m_Pool.m_Tasks.front()).m_Task;
			m_Pool.m_Tasks.pop_front();
			locker.unlock();
			(*task)();
			//Callbackje
			if (task->m_Callback != nullptr)
			{
				ITask* cBack = task->m_Callback;
				(*cBack)();
			}
		}
		else {
			locker.unlock();
		}
	}
}
