#include <iostream>
#include "Common.h"
#include "NetworkSocket.h"

using namespace mysqlpp;


int main(int argc, char *argv[])
{	
	int port = 15326;
	if (argc > 1)
	{
		std::stringstream str;
		for (int i = 1; i < argc; ++i) //Argument 0 is de binary naam './DomoticaServer', dus daarom vanaf 1
		{
			str << argv[i];
		}
		str >> port;
	}
	
	try
	{
		Connection con("DomoticaServer", 0, "root", "raspberry", 0);
		std::cout << (con.connected() == 1 ? "Succesfully connected to the database" : "Error on connecting to database!") << std::endl;
	}
	catch(...)
	{
		std::cout << "Connection to the database could not be established!" << std::endl;
	}
	cout << "MySQL Client Version: " << mysql_get_client_info() << endl;

	NetworkSocket socket(port);
	socket.RunSocketServer();
		
	std::cin.get();
	socket.StopSocketServer();
}

