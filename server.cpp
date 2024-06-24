#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

void errif(bool condition, const char *errmsg)
{
	if(condition){
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
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

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);
	bzero(&clnt_addr, sizeof(clnt_addr));
	int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_len);
	errif(clnt_sock==-1, "socket accept error");
	
	std::cout << "connect successful" << std::endl;
	std::cout << "client ip:" << inet_ntoa(clnt_addr.sin_addr) << " port:" << ntohs(clnt_addr.sin_port) << std::endl;

	while(true)
	{
		char buf[1024];
		bzero(&buf, sizeof(buf));
		ssize_t read_bytes = read(clnt_sock, buf, sizeof(buf));
		if(read_bytes > 0){
			std::cout << "from client: " << buf << std::endl;
			write(clnt_sock, buf ,sizeof(buf));
		}else if(read_bytes == 0){
			std::cout << "client disconnected" << std::endl;
			close(clnt_sock);
			break;
		}else if(read_bytes == -1){
			close(clnt_sock);
			errif(true, "socket read error");
		}
	}

	return 0;
}
