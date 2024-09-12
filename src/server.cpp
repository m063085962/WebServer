#include <iostream>
#include "TcpServer.h"
#include "SignalHandler.h"
#include "Connection.h"
#include "Buffer.h"
#include "Socket.h"

int main()
{
	TcpServer *server = new TcpServer();

	Signal::signal(SIGINT, [&] {
		delete server;
		std::cout << "\nServer exit!" << std::endl;
		exit(0);
	});

	server->OnConnect([](Connection *conn) {
		std::cout << "New connection fd: " << conn->GetSocket()->fd() << std::endl;
	});
	
	server->OnRecv([](Connection *conn) {
		std::cout << "Message from client: " << conn->GetReadBuffer()->c_str() << std::endl;
		conn->Send(conn->GetReadBuffer()->c_str());
	});

	server->Start();

	return 0;
}

