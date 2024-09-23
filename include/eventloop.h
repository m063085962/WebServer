#pragma once

#include <memory>
#include "common.h"

class EventLoop {
public:
	EventLoop();
	~EventLoop();

	void Loop() const;
	void UpdateChannel(Channel *ch) const;
	void DeleteChannel(Channel *ch) const;

private:
	DISALLOW_COPY_AND_MOVE(EventLoop);

	std::unique_ptr<Epoll> epoll_;
};

