#include "NetworkSocket.h"
#include <errno.h>

NetworkSocket::NetworkSocket(int port) : m_Port(port)
{
}


NetworkSocket::~NetworkSocket()
{
	if (m_Thread != nullptr)
	{
		StopSocketServer();
	}
	
}

auto NetworkSocket::RunSocketServer() -> void
{
	m_Thread = new thread(&NetworkSocket::RunServer, this);
}

auto NetworkSocket::StopSocketServer() -> void
{
	if (m_Thread != nullptr)
	{
		m_Stop = false;
		shutdown(m_Sock, SHUT_RDWR);
		m_Thread->join();
		delete m_Thread;
		m_Thread = nullptr;
	}
}

auto NetworkSocket::RunServer() -> void
{
	if ((m_Sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		cout << "Error on socket creation!" << endl;
		return;
	}
	
	int temp = 1;
	if (setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) < 0)
		cout << "SETSOCK OPT FAILED!" << endl;
	
	m_Server.sin_family = AF_INET; //IPV4
	m_Server.sin_port = htons(m_Port); //Host To Network byte order van port
	m_Server.sin_addr.s_addr = INADDR_ANY; //Elke IP die hij kan binden van de interfaces op deze pc, aka op elke interface luistert deze socket
	bzero(&m_Server.sin_zero, 8); //zero'en van de laatste member van de struct
	
	int len = sizeof(sockaddr_in);
	
	//binden
	if ((bind(m_Sock, (sockaddr*)&m_Server, len)) == -1)
	{
		cout << "Error binding port!" << endl;
		cout << strerror(errno) << endl;
		return;
	}
	
	if (listen(m_Sock, 50) == -1)
	{
		cout << "Error listening!" << endl;
		return;
	}
		
	cout << "Succesfully started socket server on port: " << m_Port << endl;
	
	//Client identifier
	int cli;
	int length = 0;
	
	while (m_Stop != true)
	{	
		if ((cli = accept(m_Sock, (struct sockaddr*) &m_Client, reinterpret_cast<socklen_t*>(&len))) == -1)
		{
			cout << "Error on accepting or socket has been shut down!" << endl;
			return;
		}
			
		//Vanaf hier verbonden met client!
		
		//ClientInfo fetchen
		string ClientIP([this]() -> string { char buf[INET_ADDRSTRLEN]; return string(inet_ntop(AF_INET, &m_Client.sin_addr, buf, INET_ADDRSTRLEN)); }() );
		
		cout << "A new client connected from port: " << ntohs(m_Client.sin_port) << " and IP: " << ClientIP << endl;
		/*
		//Welcome bericht sturen
		int sent;
		string message("Welcome!");
		sent = send(cli, message.c_str(), message.length(), 0 );
	
		cout << "Send " << sent << " bytes to: " << ClientIP << endl;
		*/
		//Non-Block mode aanzetten ivm multithreading
		if (fcntl(cli, F_SETFL, fcntl(cli, F_GETFL) | O_NONBLOCK) < 0) {
			cout << "Error putting socket in non block mode!" << endl;
		}
		fd_set fdset;
		struct timeval tv;
		tv.tv_sec = 1; //1sec timeout
		tv.tv_usec = 0;
		FD_ZERO(&fdset);
		FD_SET(cli, &fdset);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		//Reply receiven
		int data_len = 0;
		
		int ByteCount = 0;
		char buf[MAX_REPLY_BUFFER_SIZE];
		ioctl(cli, FIONREAD, &ByteCount);
		if (ByteCount > 0)
		{
			do
			{
				data_len = recv(cli, buf, MAX_REPLY_BUFFER_SIZE, 0);
				if(data_len != 0)
					buf[data_len] = '\0';
				//Checken of er bytes beschikbaar zijn
				ioctl(cli, FIONREAD, &ByteCount);
			} while (data_len);
		}
		
		
		cout << "Received reply: " << string(buf) << endl;	
				
		close(cli);
	}
}