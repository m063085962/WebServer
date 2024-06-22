#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);
	connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
	std::cout << "connect successful" << std::endl;
	
	std::cout << "ip: " << inet_ntoa(serv_addr.sin_addr) << std::endl;
	std::cout << "port: " << ntohs(serv_addr.sin_port) << std::endl;
	//char buffer[40];
	//read(sock, buffer, sizeof(buffer)-1);
	return 0;
}
