#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <error.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	errif(serv_sock == -1, "socket create error");

	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);
	errif(bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket bind error");

	errif(listen(serv_sock, SOMAXCONN)==-1, "socket listen error");
	std::cout << "waiting for messages" << std::endl;

	int ep = epoll_create1(0);
	errif(ep == -1, "epoll create error");

	struct epoll_event events[MAX_EVENTS], ev;
	bzero(&events, sizeof(events));
	bzero(&ev, sizeof(ev));

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = serv_sock;
	setnonblocking(serv_sock);
	epoll_ctl(ep, EPOLL_CTL_ADD, serv_sock, &ev);

	while(true){
		int nfds = epoll_wait(ep, events, MAX_EVENTS, -1);
		errif(nfds==-1, "epoll wait error");
		for(int i=0; i<nfds; i++){
			if(events[i].data.fd == serv_sock){
				struct sockaddr_in clnt_addr;
				socklen_t clnt_addr_len = sizeof(clnt_addr);
				bzero(&clnt_addr, sizeof(clnt_addr));

				int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_len);
				errif(clnt_sock==-1, "socket accept error");
				std::cout << "client ip:" << inet_ntoa(clnt_addr.sin_addr) << " port:" << ntohs(clnt_addr.sin_port) << std::endl;
				
				bzero(&ev, sizeof(ev));
				ev.data.fd = clnt_sock;
				ev.events = EPOLLIN | EPOLLET;
				setnonblocking(clnt_sock);
				epoll_ctl(ep, EPOLL_CTL_ADD, clnt_sock, &ev);
			}else if(events[i].events & EPOLLIN){
				char buf[READ_BUFFER];
				while(true){
					bzero(&buf, sizeof(buf));
					ssize_t read_bytes = read(events[i].data.fd, buf, sizeof(buf));
					if(read_bytes > 0){
						std::cout << "from client: " << buf << std::endl;
						write(events[i].data.fd, buf ,sizeof(buf));
					}else if(read_bytes == 0){
						std::cout << "EOF,client" << events[i].data.fd << " disconnected" << std::endl;
						close(events[i].data.fd);
						break;
					}else if(read_bytes==-1 && ((errno==EAGAIN) || (errno==EWOULDBLOCK))){
						std::cout << "finish reading once, errno: " << errno << std::endl;
						break;
					}else if(read_bytes==-1 && errno==EINTR){
						std::cout << "continue reading" << std::endl;
						continue;
					}
				}
			}else{
				std::cout << "something else happened" << std::endl;
			}
		}
	}
	close(serv_sock);
	return 0;
}
