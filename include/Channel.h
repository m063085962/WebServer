#pragma once

#include <functional>
#include "common.h"

class Channel
{
public:
	Channel(int fd, EventLoop *loop);
	~Channel();
	
	void HandleEvent() const;
	void EnableRead();
	void EnableWrite();
	
	int fd() const;
	short GetListenEvents() const;
	short GetReadyEvents() const;
	bool GetExist() const;
	void SetExist(bool in = true);
	void UseET();

	void SetReadyEvents(short ev);
	void SetReadCallback(std::function<void()> const &callback);
	void SetWriteCallback(std::function<void()> const &callback);

	static const int READ_EVENT;
	static const int WRITE_EVENT;
	static const int ET;

private:
	int fd_;
	EventLoop *loop_;          
	short listen_events_;
	short ready_events_;
	bool exist_;
	std::function<void()> read_callback_;
	std::function<void()> write_callback_;
};
