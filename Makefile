server:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp client.cpp -o client &&\
	g++ server.cpp -pthread \
	src/util.cpp src/Server.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp \
	src/EventLoop.cpp src/Acceptor.cpp src/Connection.cpp src/Buffer.cpp src/ThreadPool.cpp \
	-o server
clean:
	rm -rf server client
