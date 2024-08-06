#include "Acceptor.h"

#include <utility>

#include "Socket.h"
#include "Channel.h"

Acceptor::Acceptor(EventLoop *loop) : loop_(loop), sock_(nullptr), channel_(nullptr)
{
	sock_ = new Socket();
	InetAddress *addr = new InetAddress("127.0.0.1", 8888);
	sock_->Bind(addr);
	// sock_->setnonblocking();
	sock_->Listen();
	channel_ = new Channel(loop_, sock_);
	std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
	channel_->SetReadCallback(cb);
	channel_->EnableRead();
	delete addr;
}

Acceptor::~Acceptor()
{
	delete sock_;
	delete channel_;
}

void Acceptor::AcceptConnection()
{
	InetAddress *clnt_addr = new InetAddress();
	Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
	printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), clnt_addr->GetIp(), clnt_addr->GetPort());
	clnt_sock->SetNonBlocking();
	new_connection_callback_(clnt_sock);
	delete clnt_addr;
}

void Acceptor::SetNewConnectionCallback(std::function<void(Socket *)> const &callback)
{
	new_connection_callback_ = callback;
}

