#include "timer.h"

#include <utility>

Timer::Timer(Timestamp expiration, Func&& cb, double interval = 0.0)
	: expiration_(expiration),
	  callback_(std::move(cb)),
	  interval_(interval),
	  repeat_(interval > 0.0) {
}

void Timer::Restart(Timestamp now) {
	expiration_ = Timestamp::AddTime(now, interval_);
}

void Timer::Run() const { callback_(); }

Timestamp Timer::expiration() const { return expiration_; }

bool Timer::repeat() const { return repeat_; }

