#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Acceptor.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : loop(_loop)
{
	acceptor = new Acceptor(loop);
	std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
	acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
	delete acceptor;
}

void Server::handleReadEvent(int sockfd)
{
	char buf[READ_BUFFER];
	while(true){
		bzero(&buf, sizeof(buf));
		ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
		if(bytes_read > 0){
			std::cout << "message from client " << sockfd << ": " << buf << std::endl;
			write(sockfd, buf, sizeof(buf));
		} else if(bytes_read==-1 && errno==EINTR){
			std::cout << "continue reading" << std::endl;
			continue;
		} else if(bytes_read==-1 && ((errno==EAGAIN)||(errno==EWOULDBLOCK))){
			std::cout << "finish reading once, errno: " << errno << std::endl;
			break;
		} else if(bytes_read==0){
			std::cout << "EOF, client " << sockfd << " disconnected" << std::endl;
			close(sockfd);
			break;
		}
	}
}

void Server::newConnection(Socket *serv_sock){
	InetAddress *clnt_addr = new InetAddress();
	Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
	std::cout << "new client fd " << clnt_sock->getFd();
	std::cout << " IP: " << inet_ntoa(clnt_addr->addr.sin_addr);
	std::cout << " Port: " << ntohs(clnt_addr->addr.sin_port) << std::endl;
	clnt_sock->setnonblocking();
	Channel *clntChannel = new Channel(loop, clnt_sock->getFd());
	std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
	clntChannel->setCallback(cb);
	clntChannel->enableReading();
}

