#pragma once

class Threadpool;

//Worker class
class Worker {
public:
	Worker(Threadpool &s) : m_Pool(s) {}
	void operator()();
private:
	Threadpool& m_Pool;
};

