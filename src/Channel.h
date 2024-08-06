#pragma once

#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
	EventLoop *loop_;
	Socket *socket_;
	uint32_t listen_events_{0};
	uint32_t ready_events_{0};
	bool exist_{false};
	std::function<void()> read_callback_;
	std::function<void()> write_callback_;

public:
	Channel(EventLoop *loop, Socket *socket);
	~Channel();
	
	void HandleEvent();
	void EnableRead();
	void EnableWrite();
	
	Socket *GetSocket();
	int GetListenEvents();
	int GetReadyEvents();
	bool GetExist();
	void SetExist(bool in = true);
	void UseET();

	void SetReadyEvents(int ev);
	void SetReadCallback(std::function<void()> const &callback);
	void SetWriteCallback(std::function<void()> const &callback);

	static const int READ_EVENT;
	static const int WRITE_EVENT;
	static const int ET;
};

