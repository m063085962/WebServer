#include "acceptor.h"

#include <cassert>
#include <utility>
#include <fcntl.h>

#include "socket.h"
#include "channel.h"

Acceptor::Acceptor(EventLoop *loop)
{
	socket_ = std::make_unique<Socket>();
	assert(socket_->Create() == RC_SUCCESS);
	assert(socket_->Bind("127.0.0.1", 8888) == RC_SUCCESS);
	assert(socket_->Listen() == RC_SUCCESS);

	channel_ = std::make_unique<Channel>(socket_->fd(), loop);
	std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);

	channel_->SetReadCallback(cb);
	channel_->EnableRead();
}

Acceptor::~Acceptor() {}

RC Acceptor::AcceptConnection() const
{
	int clnt_fd = -1;
	if(socket_->Accept(clnt_fd) != RC_SUCCESS){
		return RC_ACCEPTOR_ERROR;
	}
	fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK); // NonBlocking
	if(new_connection_callback_){
		new_connection_callback_(clnt_fd);
	}
	return RC_SUCCESS;
}

void Acceptor::SetNewConnectionCallback(std::function<void(int)> const &callback)
{
	new_connection_callback_ = std::move(callback);
}

