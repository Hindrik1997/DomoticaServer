#include <iostream>
#include "Common.h"
#include "NetworkSocket.h"
#include "CommandExecutionEngine.h"

using namespace mysqlpp;

int main(int argc, char *argv[])
{	
	//WiringPiSetup 
	wiringPiSetupGpio();
	digitalWrite(6, LOW);
	digitalWrite(13, LOW);
	digitalWrite(19, LOW);
	
	cout << endl;
	cout << "----------------------------------" << endl;
	cout << "DomoticaServer Version 1.0.0" << endl;
	cout << "----------------------------------" << endl;
	cout << endl;
	cout << "----------------------------------" << endl;
	cout << "LOG/TERMINAL: " << endl;
	cout << "Use the 'exit' command to shutdown the server." << endl;
	cout << "----------------------------------" << endl;
	cout << endl;
	cout << endl;
	
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
	
	Connection con("DomoticaServer", 0, "root", "raspberry", 0);
	std::cout << (con.connected() == 1 ? "Succesfully connected to the database" : "Error on connecting to database!") << std::endl;
	cout << "MySQL Client Version: " << mysql_get_client_info() << endl;

	CommandExecutionEngine cee(std::thread::hardware_concurrency(), con);
	NetworkSocket socket(port, cee);
	socket.RunSocketServer();
	//Voor lokale commands op de server
	while (true)
	{
		char buf[128];
		cin.getline(buf,128);
		string cmd(buf);
		if (cmd == "exit")
			break;
		cee.Execute(cmd, "localhost");
	}	
	socket.StopSocketServer();
}