#pragma once

#include <sys/epoll.h>
#include <vector>
#include "common.h"

class Epoll {
public:
	Epoll();
	~Epoll();

	RC UpdateChannel(Channel *ch);
	RC DeleteChannel(Channel *ch);

	std::vector<Channel*> Poll(int timeout = -1);

private:
	DISALLOW_COPY_AND_MOVE(Epoll);

	int epfd_;
	struct epoll_event *events_{nullptr};
};

