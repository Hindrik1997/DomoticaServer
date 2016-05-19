#include "Threadpool.h"

Threadpool::Threadpool(size_t threads) : m_Stop(false)
{
	for (size_t i = 0; i < threads; ++i)
		m_Workers.push_back(std::thread(Worker(*this)));
}

Threadpool::~Threadpool()
{
	m_Stop = true;
	m_Cond.notify_all();
	for (auto &thread : m_Workers)
		thread.join();
}

void Threadpool::EmptyTasks()
{
	std::unique_lock<std::mutex> lock(m_Queue_Mutex);
	if (m_Tasks.empty())
	{
		std::deque<ITaskWrapper> tDeck;
		m_Tasks.swap(tDeck);
	}
}

void Threadpool::ClearDequeRam()
{
	std::unique_lock<std::mutex> lock(m_Queue_Mutex);
	std::deque<ITaskWrapper> tDeck;
	m_Tasks.swap(tDeck);
}



void Threadpool::Enqueue(ITask& Job)
{
	std::unique_lock<std::mutex> lock(m_Queue_Mutex);
	m_Tasks.push_back(ITaskWrapper(Job));
	m_Cond.notify_one();
}
