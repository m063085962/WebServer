#pragma once

#include <string>
#include "common.h"

class Socket {
public:
	Socket();
	~Socket();

	void set_fd(int fd);

	int fd() const;
	std::string get_addr() const;

	RC Create();
	RC Bind(const char *ip, uint16_t port) const;
	RC Listen() const;
	RC Accept(int &clnt_fd) const;
	RC Connect(const char *ip, uint16_t port) const;
	RC SetNonBlocking() const;

	bool IsNonBlocking() const;
	size_t RecvBufSize() const;

private:
	DISALLOW_COPY_AND_MOVE(Socket);

	int fd_;
};

