#include "Task.h"
#include "Threadpool.h"

Task::Task(function<void()> task) : m_Task(task)
{
}

void Task::Execute(Threadpool& pool) 
{
	pool.Enqueue(*this);
}

Task::~Task()
{
}