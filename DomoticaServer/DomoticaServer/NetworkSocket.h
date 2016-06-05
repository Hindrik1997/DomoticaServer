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
#define SEND_PORT 15327

class CommandExecutionEngine;

//Class for easily starting a server on a POSIX/Berkeley socket
class NetworkSocket
{
public:
	NetworkSocket(int port, CommandExecutionEngine& execEngine);
	~NetworkSocket();
	static bool SendMessage(string ip, string message); //Uses port 'SEND_PORT' (MACRO in NetworkSocket.h) to send!
	void RunSocketServer(); //Starts the socket server in a secondary thread
	void StopSocketServer(); //Stops the socket server, after this call you can reuse this object by calling RunSocketServer() 
private:
	bool m_Stop;
	std::thread* m_Thread = nullptr;
	int m_Port;
	int m_Sock_fd;
	CommandExecutionEngine& m_ExecEngine;
private: 
	void RunServer();
};

