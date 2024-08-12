#include "EventLoop.h"

#include <vector>

#include "Epoll.h"
#include "Channel.h"


EventLoop::EventLoop() {epoll_ = std::make_unique<Epoll>();}

EventLoop::~EventLoop() {}

void EventLoop::Loop() const
{
	while(true){
		for(Channel *active_ch : epoll_->Poll()){
			active_ch->HandleEvent();
		}
	}
}

void EventLoop::UpdateChannel(Channel *ch) const {epoll_->UpdateChannel(ch);}

void EventLoop::DeleteChannel(Channel *ch) const {epoll_->DeleteChannel(ch);}

