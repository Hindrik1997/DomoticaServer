#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Common.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define MAX_REPLY_BUFFER_SIZE 1024
#define MAX_CONNECTIONS 5

class CommandExecutionEngine;

//Class for easily starting a server on a POSIX/Berkeley socket
class NetworkSocket
{
public:
	NetworkSocket(int port, CommandExecutionEngine& execEngine);
	~NetworkSocket();
	void RunSocketServer(); //Starts the socket server in a secondary thread
	void StopSocketServer(); //Stops the socket server, after this call you can reuse this object by calling RunSocketServer() 
private:
	bool m_Stop;
	std::thread* m_Thread = nullptr;
	void RunServer();
	void RunServerOld();//Old variant of RunServer(). Do NOT USE!
	int m_Port;
	int m_Sock_fd;
	CommandExecutionEngine& m_ExecEngine;
};

