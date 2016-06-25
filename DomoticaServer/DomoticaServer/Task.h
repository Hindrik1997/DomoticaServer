#pragma once
#include "ITask.h"
#include <functional>

using std::function;
class Threadpool;

//Class voor het vergemakkelijken van het gebruik van de ITask class
class Task : public ITask
{
public:
	Task(function<void()> task);
	~Task();
private:
	function<void()> m_Task;
public:
	void Execute(Threadpool& pool);
	inline void operator()();
};

inline void Task::operator()() { m_Task(); }