#include <iostream>
#include "Socket.h"
#include "Connection.h"
#include "Buffer.h"

int main()
{
	Socket *sock = new Socket();
	sock->Create();
	sock->Connect("127.0.0.1", 8888);

	Connection *conn = new Connection(sock->fd(), nullptr);

	while(true){
		std::string input;
		std::getline(std::cin, input);
		conn->SetSendBuffer(input.c_str());
		conn->Write();
		if(conn->GetState() == Connection::State::Closed){
			conn->Close();
			break;
		}
		conn->Read();
		std::cout << "Message from server: " << conn->GetReadBuffer()->c_str() << std::endl;
	}

	delete sock;
	return 0;
}

