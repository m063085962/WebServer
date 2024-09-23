#pragma once

#include <functional>

#include "common.h"
#include "timestamp.h"

class Timer {
public:
	typedef std::function<void()> Func;

	Timer(Timestamp timestamp, Func&& cb, double interval);

	void Restart(Timestamp now);
	void Run() const;
	Timestamp expiration() const;
	bool repeat() const;

private:
	DISALLOW_COPY_AND_MOVE(Timer);

	Timestamp expiration_;
	Func callback_;
	double interval_;
	bool repeat_;
};

