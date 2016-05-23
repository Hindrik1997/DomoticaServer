#include <iostream>
#include "Common.h"
#include "NetworkSocket.h"

using namespace mysqlpp;


int main(int argc, char *argv[])
{	
	int port = 15326;
	if (argc > 0)
	{
		std::stringstream str;
		for (int i = 0; i < argc; ++i)
		{
			str << argv[i];
		}
		str >> port;
	}
	
	
	NetworkSocket socket(port);
	socket.RunSocketServer();
	try
	{
		Connection con("DomoticaServer", 0, "root", "raspberry", 0);
		std::cout << (con.connected() == 1 ? "Succesfully connected to the database" : "Error on connecting to database!") << std::endl;
	}
	catch(...)
	{
		std::cout << "Connection could not be established!" << std::endl;
	}
	
	printf("MySQL client version: %s\n", mysql_get_client_info());
		
	std::cin.get();
	socket.StopSocketServer();
}

