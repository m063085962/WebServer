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
	bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(sock, SOMAXCONN);

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);
	bzero(&clnt_addr, sizeof(clnt_addr));
	std::cout << "waiting connection..." << std::endl;
	int clnt_sock = accept(sock, (sockaddr*)&clnt_addr, &clnt_addr_len);

	std::cout << "ip: " << inet_ntoa(clnt_addr.sin_addr) << std::endl;
	std::cout << "port: " << ntohs(clnt_addr.sin_port) << std::endl;

	return 0;
}
