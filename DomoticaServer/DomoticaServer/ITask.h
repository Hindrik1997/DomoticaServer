#pragma once

//Task interface.
class ITask
{
public:
	virtual void SetCallback(ITask* callback) { m_Callback = callback; };
	virtual void operator()() = 0; //Pure virtuele functie. Maakt de ITask class een abstracte class. (Deze class is bij deze nu dus een soort C# Interface)
	ITask* m_Callback = nullptr; //Callback support
};