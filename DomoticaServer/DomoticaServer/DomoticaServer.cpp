#include <iostream>
#include "Common.h"

using namespace mysqlpp;

int main(int argc, char *argv[])
{	
	try
	{
		Connection con("DomoticaServer", 0, "root", "raspberry", 0);
		std::cout << con.connected() << std::endl;
	}
	catch(...)
	{
		std::cout << "Connection could not be established!" << std::endl;
	}
	
	printf("MySQL client version: %s\n", mysql_get_client_info());
		
	std::cin.get();
}