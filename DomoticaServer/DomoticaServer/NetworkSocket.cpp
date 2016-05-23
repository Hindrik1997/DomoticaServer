#include "NetworkSocket.h"

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
	
	m_Server.sin_family = AF_INET; //IPV4
	m_Server.sin_port = htons(m_Port); //Host To Network byte order van port
	m_Server.sin_addr.s_addr = INADDR_ANY; //Elke IP die hij kan binden van de interfaces op deze pc
	bzero(&m_Server.sin_zero, 8); //zero'en van de laatste member van de struct
	
	int len = sizeof(sockaddr_in);
	
	//binden
	if ((bind(m_Sock, (sockaddr*)&m_Server, len)) == -1)
	{
		cout << "Error binding port!" << endl;
		return;
	}
	
	if (listen(m_Sock, 50) == -1)
	{
		cout << "Error listening!" << endl;
		return;
	}
	
	string ServerIP([this]() -> string { char buf[INET_ADDRSTRLEN]; return string(inet_ntop(AF_INET, &m_Server.sin_addr, buf, INET_ADDRSTRLEN)); }());
	
	
	cout << "Succesfully started socket server on: " << ServerIP << " and port: " << m_Port << endl;
	
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
			
		//ClientInfo fetchen
		string ClientIP([this]() -> string { char buf[INET_ADDRSTRLEN]; return string(inet_ntop(AF_INET, &m_Client.sin_addr, buf, INET_ADDRSTRLEN)); }() );
		
		cout << "A new client connected from port: " << ntohs(m_Client.sin_port) << " and IP: " << ClientIP << endl;
		
		
		int sent;
		int received;
		//Verbonden met client!
		string message("Welcome!");
		
		sent = send(cli, message.c_str(), message.length(), 0 );
		
		
	
		cout << "Send " << sent << " bytes to: " << ClientIP << endl;
		
		close(cli);
	}
}