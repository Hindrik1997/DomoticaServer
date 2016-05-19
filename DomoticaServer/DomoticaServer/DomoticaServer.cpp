#include <iostream>
#include "Common.h"


using namespace std;
Threadpool pool(std::thread::hardware_concurrency());

void test()
{
	cout << "TEST" << endl;
}

int main(int argc, char *argv[])
{	
	Task t(test);
	for (int i = 0; i < 1000; ++i)
	{
		t.Execute(pool);
	}
	cin.get();
}