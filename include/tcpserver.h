#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "common.h"

class TcpServer {
public:
	TcpServer();
	~TcpServer();

	void Start();

	RC NewConnection(int fd);
	RC DeleteConnection(int fd);

	void OnConnect(std::function<void(Connection *)> fn);
	void OnRecv(std::function<void(Connection *)> fn);

private:
	DISALLOW_COPY_AND_MOVE(TcpServer);

	std::unique_ptr<EventLoop> main_reactor_;
	std::unique_ptr<Acceptor> acceptor_;

	std::unordered_map<int, std::unique_ptr<Connection>> connections_;
	std::vector<std::unique_ptr<EventLoop>> sub_reactors_;
	
	std::unique_ptr<ThreadPool> thread_pool_;

	std::function<void(Connection *)> on_connect_;
	std::function<void(Connection *)> on_recv_;
};

