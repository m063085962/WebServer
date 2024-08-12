src = $(wildcard src/*.cpp)
server:
	g++ -std=c++17 -pthread -g \
	$(src) \
	server.cpp \
	-o server \
	&& \
	g++ -std=c++17 -g \
	src/Connection.cpp src/EventLoop.cpp src/Channel.cpp src/Socket.cpp src/Epoll.cpp src/Buffer.cpp \
	client.cpp \
	-o client

test:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/ThreadPool.cpp \
	-pthread \
	test.cpp -o test
	
clean:
	rm -rf server client test
