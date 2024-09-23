#include "timerqueue.h"

#include <unistd.h>
#include <sys/timerfd.h>

#include <cassert>
#include <cstring>
#include <memory>

#include "timer.h"
#include "channel.h"
#include "eventloop.h"

TimerQueue::TimerQueue(EventLoop* loop)
		: loop_(loop),
		  timerfd_(::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)),
		  channel_(new Channel(timerfd_, loop_)) {
	channel_->SetReadCallback(std::bind(&TimerQueue::HandleRead, this));
	channel_->EnableRead();
}

TimerQueue::~TimerQueue() {
	if (timerfd_ != -1) {
		close(timerfd_);
		timerfd_ = -1;
	}
	for (const auto& timerpair : timers_) {
		delete timerpair.second;
	}
}

void TimerQueue::ReadTimerFd() {
	uint64_t read_byte;
	ssize_t readn = ::read(timerfd_, &read_byte, sizeof(read_byte));
	if(readn != sizeof(read_byte)) {
		perror("ReadTimerFd read SYS_ERR\n");
	}
}

void TimerQueue::HandleRead() {
	ReadTimerFd();
	Timestamp expiration_time(Timestamp::Now());

	active_timers_.clear();
	auto end = timers_.lower_bound(TimerPair(Timestamp::Now(), reinterpret_cast<Timer*>(UINTPTR_MAX)));
	timers_.erase(timers_.begin(), end);

	for (const auto& timerpair : active_timers_) {
		timerpair.second->Run();
	}
	ResetTimers();
}

void TimerQueue::ResetTimers() {
	for (auto& timerpair : active_timers_) {
		if ((timerpair.second)->repeat()) {
			auto timer = timerpair.second;
			timer->Restart(Timestamp::Now());
			Insert(timer);
		} else {
			delete timerpair.second;
		}
	}
	if (!timers_.empty()) {
		ResetTimer(timers_.begin()->second);
	}
}

bool TimerQueue::Insert(Timer* timer) {
	bool reset_instantly = timers_.empty() || (timer->expiration() < timers_.begin()->first);
	timers_.emplace(std::move(TimerPair(timer->expiration(), timer)));
	return reset_instantly;
}

void TimerQueue::AddTimerInLoop(Timer* timer) {
	bool reset_instantly = Insert(timer);
	if (reset_instantly) {
		ResetTimer(timer);
	}
}

void TimerQueue::ResetTimer(Timer* timer) {
	struct itimerspec new_;
	struct itimerspec old_;
	memset(&new_, '\0', sizeof(new_));
	memset(&old_, '\0', sizeof(old_));

	int64_t micro_seconds_dif = timer->expiration().microseconds() - Timestamp::Now().microseconds();
	if (micro_seconds_dif < 100) {
		micro_seconds_dif = 100;
	}

	new_.it_value.tv_sec = static_cast<time_t>(micro_seconds_dif / kMicrosecond2Second);
	new_.it_value.tv_nsec = static_cast<long>((micro_seconds_dif % kMicrosecond2Second) * 1000);
	int ret = ::timerfd_settime(timerfd_, 0, &new_, &old_);
	assert(ret != -1);
	(void) ret;
}

void TimerQueue::AddTimer(Timestamp timestamp, Func&& cb, double interval) {
	Timer* timer(new Timer(timestamp, std::move(cb), interval));
	// loop_->RunOneFunc(std::bind(&TimerQueue::AddTimerInLoop, this, timer));
}

