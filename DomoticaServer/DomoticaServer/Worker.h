#pragma once

class Threadpool;

class Worker {
public:
	Worker(Threadpool &s) : m_Pool(s) {}
	void operator()();
private:
	Threadpool& m_Pool;
};

