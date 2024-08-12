#include "Channel.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <utility>

#include "EventLoop.h"
#include "Socket.h"

const int Channel::READ_EVENT = 1;
const int Channel::WRITE_EVENT = 2;
const int Channel::ET = 4;

Channel::Channel(int fd, EventLoop *loop) : fd_(fd), loop_(loop), listen_events_(0), ready_events_(0), exist_(false) {}

Channel::~Channel() {loop_->DeleteChannel(this);}

void Channel::HandleEvent() const
{
	if(ready_events_ & READ_EVENT){
		read_callback_();
	}
	if(ready_events_ & WRITE_EVENT){
		write_callback_();
	}
}

void Channel::EnableRead()
{
	listen_events_ |= READ_EVENT;
	loop_->UpdateChannel(this);
}

void Channel::EnableWrite()
{
	listen_events_ |= WRITE_EVENT;
	loop_->UpdateChannel(this);
}

void Channel::UseET()
{
	listen_events_ |= ET;
	loop_->UpdateChannel(this);
}

int Channel::fd() const {return fd_;}

short Channel::GetListenEvents() const {return listen_events_;}
short Channel::GetReadyEvents() const {return ready_events_;}

bool Channel::GetExist() const {return exist_;}

void Channel::SetExist(bool in) {exist_ = in;}

void Channel::SetReadyEvents(short ev) 
{
	if(ev & READ_EVENT){
		ready_events_ |= READ_EVENT;
	}
	if(ev & WRITE_EVENT){
		ready_events_ |= WRITE_EVENT;
	}
	if(ev & ET){
		ready_events_ |= ET;
	}
}

void Channel::SetReadCallback(std::function<void()> const &callback) {read_callback_ = std::move(callback);}
void Channel::SetWriteCallback(std::function<void()> const &callback) {write_callback_ = std::move(callback);}

