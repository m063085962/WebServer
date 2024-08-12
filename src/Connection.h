#pragma once

#include <functional>
#include <memory>
#include "common.h"

class Connection
{
public:
	enum State{
		Invalid = 0,
		Connecting,
		Connected,
		Closed,
	};
	Connection(int fd, EventLoop *loop);
	~Connection();
	
	RC Read();
	RC Write();
	RC Send(std::string msg);

	void SetDeleteConnection(std::function<void(int)> const &fn);
	void SetOnConnect(std::function<void(Connection *)> const &fn);
	void SetOnRecv(std::function<void(Connection *)> const &fn);

	State GetState() const;
	Socket *GetSocket() const;
	void SetSendBuffer(const char *str);
	Buffer *GetReadBuffer();
	Buffer *GetSendBuffer();

	void Close();

	void OnConnect(std::function<void()> fn);
	void OnMessage(std::function<void()> fn);

private:
	void Business();
	RC ReadNonBlocking();
	RC WriteNonBlocking();
	RC ReadBlocking();
	RC WriteBlocking();

private:
	std::unique_ptr<Socket> socket_;
	std::unique_ptr<Channel> channel_;

	State state_;
	std::unique_ptr<Buffer> read_buf_;
	std::unique_ptr<Buffer> send_buf_;

	std::function<void(int)> delete_connection_;
	std::function<void(Connection *)> on_recv_;
};

