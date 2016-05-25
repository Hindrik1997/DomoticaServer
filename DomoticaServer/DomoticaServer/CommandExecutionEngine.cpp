#include "CommandExecutionEngine.h"



CommandExecutionEngine::CommandExecutionEngine(size_t threadcount) : pool(threadcount - 1)
{
}


CommandExecutionEngine::~CommandExecutionEngine()
{
}

void CommandExecutionEngine::Execute(string command)
{
	
}