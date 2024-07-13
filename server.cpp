#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)|O_NONBLOCK);
}
void handleReadEvent(int);

int main()
{
	Socket *serv_sock = new Socket();
	InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
	serv_sock->bind(serv_addr);
	serv_sock->listen();
	Epoll *ep = new Epoll();
	serv_sock->setnonblocking();
	ep->addFd(serv_sock->getFd(), EPOLLIN|EPOLLET);
	while(true){
		std::vector<epoll_event> events = ep->poll();
		int nfds = events.size();
		for(int i=0; i<nfds; i++){
			if(events[i].data.fd == serv_sock->getFd()){
				InetAddress *clnt_addr = new InetAddress();
				Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
				std::cout << "new client fd:" << clnt_sock->getFd();
				std::cout << " IP:" << inet_ntoa(clnt_addr->addr.sin_addr);
				std::cout << " Port:" << ntohs(clnt_addr->addr.sin_port) << std::endl;
				clnt_sock->setnonblocking();
				ep->addFd(clnt_sock->getFd(), EPOLLIN|EPOLLET);
			}else if(events[i].events & EPOLLIN){
				handleReadEvent(events[i].data.fd);
			}else{
				std::cout << "something else happened" << std::endl;
			}
		}
	}
	delete serv_sock;
	delete serv_addr;
	return 0;
}

void handleReadEvent(int sockfd)
{
	char buf[READ_BUFFER];
	while(true){
		bzero(&buf, sizeof(buf));
		ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
		if(read_bytes > 0){
			std::cout << "from client " << sockfd << ": " << buf << std::endl;
			write(sockfd, buf ,sizeof(buf));
		}else if(read_bytes == 0){
			std::cout << "EOF,client " << sockfd << " disconnected" << std::endl;
			close(sockfd);
			break;
		}else if(read_bytes==-1 && ((errno==EAGAIN) || (errno==EWOULDBLOCK))){
			std::cout << "finish reading once, errno: " << errno << std::endl;
			break;
		}else if(read_bytes==-1 && errno==EINTR){
			std::cout << "continue reading" << std::endl;
			continue;
		}
	}
}

