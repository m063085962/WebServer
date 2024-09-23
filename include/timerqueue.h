#pragma once

#include <set>
#include <vector>
#include <utility>
#include <memory>
#include <functional>

#include "common.h"
#include "timestamp.h"

class TimerQueue {
public:
	typedef std::function<void()> Func;
	
	TimerQueue(EventLoop* loop);
	~TimerQueue();

	void ReadTimerFd();
	void HandleRead();
	void ResetTimers();
	bool Insert(Timer* timer);
	void AddTimerInLoop(Timer* timer);

	void ResetTimer(Timer* timer);
	void AddTimer(Timestamp timestamp, Func&& cb, double interval);

private:
	DISALLOW_COPY_AND_MOVE(TimerQueue);

	typedef std::pair<Timestamp, Timer*> TimerPair;

	EventLoop* loop_;
	int timerfd_;
	std::unique_ptr<Channel> channel_;
	
    std::set<TimerPair> timers_;
    std::vector<TimerPair> active_timers_;
};

