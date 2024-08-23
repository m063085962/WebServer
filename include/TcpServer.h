#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include "common.h"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();
	void Start();

	RC NewConnection(int fd);
	RC DeleteConnection(int fd);

	void OnConnect(std::function<void(Connection *)> fn);
	void OnRecv(std::function<void(Connection *)> fn);

private:
	std::unique_ptr<EventLoop> main_reactor_;
	std::unique_ptr<Acceptor> acceptor_;

	std::vector<std::unique_ptr<EventLoop>> sub_reactors_;
	std::unordered_map<int, std::unique_ptr<Connection>> connections_;
	
	std::unique_ptr<ThreadPool> thread_pool_;

	std::function<void(Connection *)> on_connect_;
	std::function<void(Connection *)> on_recv_;
};

