#include <iostream>
#include "tcpserver.h"
#include "connection.h"
#include "buffer.h"
#include "socket.h"

int main()
{
	TcpServer *server = new TcpServer();

	server->OnConnect([](Connection *conn) {
		std::cout << "New connection fd: " << conn->GetSocket()->fd() << std::endl;
	});
	
	server->OnRecv([](Connection *conn) {
		std::cout << "Message from client: " << conn->GetReadBuffer()->c_str() << std::endl;
		conn->Send(conn->GetReadBuffer()->c_str());
	});

	server->Start();
	delete server;

	return 0;
}

