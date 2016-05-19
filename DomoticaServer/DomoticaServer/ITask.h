#pragma once

class ITask
{
public:
	virtual void SetCallback(ITask* callback) { m_Callback = callback; };
	virtual void operator()() = 0;
	ITask* m_Callback = nullptr;
};