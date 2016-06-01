#pragma once
#include "Common.h"
#include "Threadpool.h"
#include "Task.h"

using namespace mysqlpp;


class CommandExecutionEngine
{
public:
	CommandExecutionEngine(size_t threadcount, Connection& con);
	~CommandExecutionEngine();
	void Execute(string command);
private:
	Connection& conref;
	Threadpool pool;
};

