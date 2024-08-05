#pragma once
#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

class Channel;
class Epoll
{
private:
	int epfd_{-1};
	struct epoll_event *events_{nullptr};

public:
	Epoll();
	~Epoll();

	void UpdateChannel(Channel *ch);
	void DeleteChannel(Channel *ch);

	std::vector<Channel *> Poll(int timeout = -1);
};

