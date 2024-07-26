#include <iostream>
#include <string.h>
#include <unistd.h>
#include "src/util.h"
#include "src/Buffer.h"
#include "src/InetAddress.h"
#include "src/Socket.h"

using namespace std;

int main()
{
	Socket *sock = new Socket();
	InetAddress *addr = new InetAddress("127.0.0.1", 8888);
	sock->connect(addr);

	int sockfd = sock->getFd();

	Buffer *sendBuffer = new Buffer();
	Buffer *readBuffer = new Buffer();

	while(true){
		sendBuffer->getline();
		ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
		if(write_bytes == -1){
			cout << "socket already disconnected, can't write any more!" << endl;
			break;
		}
		int already_read = 0;
		char buf[1024];
		while(true){
			bzero(&buf, sizeof(buf));
			ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
			if(read_bytes > 0){
				readBuffer->append(buf, read_bytes);
				already_read += read_bytes;
			}else if(read_bytes == 0){
				cout << "server disconnected!" << endl;
				exit(EXIT_SUCCESS);
			}
			if(already_read >= sendBuffer->size()){
				printf("message from server: %s\n", readBuffer->c_str());
				break;
			}
		}
		readBuffer->clear();
	}
	delete addr;
	delete sock;
	return 0;
}

