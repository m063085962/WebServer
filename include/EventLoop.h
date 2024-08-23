#pragma once

#include <memory>
#include "common.h"

class EventLoop
{
public:
	EventLoop();
	~EventLoop();

	void Loop() const;
	void UpdateChannel(Channel *ch) const;
	void DeleteChannel(Channel *ch) const;

private:
	std::unique_ptr<Epoll> epoll_;
};

