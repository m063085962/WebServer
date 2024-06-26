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
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	errif(sock==-1, "socket create error");

	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);
	errif(connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))==-1, "socket connect error");

	std::cout << "connect successful" << std::endl;
	std::cout << "server ip:" << inet_ntoa(serv_addr.sin_addr) << " port:" << ntohs(serv_addr.sin_port) << std::endl;

	while(true)
	{
		char buf[1024];
		bzero(&buf, sizeof(buf));
		std::cin >> buf;
		ssize_t write_bytes = write(sock, buf, sizeof(buf));
		if(write_bytes == -1){
			std::cout << "socket disconnected, can't write any more!" << std::endl;
			break;
		}
		bzero(&buf, sizeof(buf));
		ssize_t read_bytes = read(sock, buf, sizeof(buf));
		if(read_bytes > 0){
			std::cout << "from server: " << buf << std::endl;
		}else if(read_bytes == 0){
			std::cout << "server socket disconnected" << std::endl;
			break;
		}else if(read_bytes == -1){
			close(sock);
			errif(true, "socket read error");
		}
	}
	
	return 0;
}
