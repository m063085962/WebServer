#include "src/Server.h"
#include <iostream>
#include "src/Buffer.h"
#include "src/Connection.h"
#include "src/EventLoop.h"
#include "src/Socket.h"
#include <iostream>

int main()
{
	EventLoop *loop = new EventLoop();
	Server *server = new Server(loop);

	server->NewConnect(
		[](Connection *conn) {std::cout << "New connection fd: " << conn->GetSocket()->GetFd() << std::endl;});
	
	server->OnMessage([](Connection *conn) {
		std::cout << "Message from client " << conn->ReadBuffer() << std::endl;
		if(conn->GetState() == Connection::State::Connected){
			conn->Send(conn->ReadBuffer());
		}
	});

	loop->Loop();
	delete server;
	delete loop;
	return 0;
}

