server:
	g++ util.cpp server.cpp -o server &&\
	g++ util.cpp client.cpp -o client
clean:
	rm -rf server client
