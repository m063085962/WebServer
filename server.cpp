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
#include "Channel.h"

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
	Channel *servChannel = new Channel(ep, serv_sock->getFd());
	servChannel->enableReading();
	while(true){
		std::vector<Channel*> activeChannel = ep->poll();
		int nfds = activeChannel.size();
		for(int i=0; i<nfds; i++){
			int chfd = activeChannel[i]->getFd();
			if(chfd == serv_sock->getFd()){
				InetAddress *clnt_addr = new InetAddress();
				Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
				std::cout << "new client fd:" << clnt_sock->getFd();
				std::cout << " IP:" << inet_ntoa(clnt_addr->addr.sin_addr);
				std::cout << " Port:" << ntohs(clnt_addr->addr.sin_port) << std::endl;
				clnt_sock->setnonblocking();
				Channel *clntChannel = new Channel(ep, clnt_sock->getFd());
				clntChannel->enableReading();
			}else if(activeChannel[i]->getRevents() & EPOLLIN){
				handleReadEvent(activeChannel[i]->getFd());
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

