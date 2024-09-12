#pragma once

#include <functional>
#include <memory>
#include "common.h"

class Acceptor {
public:
	explicit Acceptor(EventLoop *loop);
	~Acceptor();

	RC AcceptConnection() const;
	void SetNewConnectionCallback(std::function<void(int)> const &callback);

private:
	DISALLOW_COPY_AND_MOVE(Acceptor);

	std::unique_ptr<Socket> socket_;
	std::unique_ptr<Channel> channel_;

	std::function<void(int)> new_connection_callback_;
};

