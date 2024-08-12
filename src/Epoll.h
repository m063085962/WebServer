#pragma once
#include <vector>
#include "common.h"

#include <sys/epoll.h>

class Channel;
class Epoll
{
public:
	Epoll();
	~Epoll();

	RC UpdateChannel(Channel *ch);
	RC DeleteChannel(Channel *ch);

	std::vector<Channel *> Poll(int timeout = -1);

private:
	int epfd_;
	struct epoll_event *events_{nullptr};
};

