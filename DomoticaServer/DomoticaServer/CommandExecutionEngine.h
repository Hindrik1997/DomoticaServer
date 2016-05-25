#pragma once
#include "Common.h"
#include "Threadpool.h"
#include "Task.h"

class CommandExecutionEngine
{
public:
	CommandExecutionEngine(size_t threadcount);
	~CommandExecutionEngine();
	void Execute(string command);
private:
	Threadpool pool;
};

