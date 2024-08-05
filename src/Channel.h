#pragma once

#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
	EventLoop *loop_;
	int fd_;
	uint32_t listen_events_;
	uint32_t ready_events_;
	bool in_epoll_;
	std::function<void()> read_callback_;
	std::function<void()> write_callback_;

public:
	Channel(EventLoop *loop, int fd);
	~Channel();
	
	void HandleEvent();
	void EnableRead();

	int GetFd();
	uint32_t GetListenEvents();
	uint32_t GetReadyEvents();
	bool GetInEpoll();
	void SetInEpoll(bool in = true);
	void UseET();

	void SetReadyEvents(uint32_t ev);
	void SetReadCallback(std::function<void()> const &callback);
};

