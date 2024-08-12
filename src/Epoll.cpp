#include "Epoll.h"

#include <unistd.h>
#include <cstring>
#include <cassert>

#include "Channel.h"
#include "Socket.h"

#define MAX_EVENTS 1000

Epoll::Epoll()
{
	epfd_ = epoll_create1(0);
	assert(epfd_ != -1);
	events_ = new epoll_event[MAX_EVENTS];
	memset(events_, 0, sizeof(*events_)*MAX_EVENTS);
}

Epoll::~Epoll()
{
	if(epfd_ != -1){
		close(epfd_);
	}
	delete[] events_;
}

std::vector<Channel*> Epoll::Poll(int timeout)
{
	std::vector<Channel *> active_channels;
	int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
	if(nfds==-1) perror(" epoll wait error");
	for(int i=0; i<nfds; i++){
		Channel *ch = (Channel *)events_[i].data.ptr;
		int events = events_[i].events;
		if(events & EPOLLIN){
			ch->SetReadyEvents(Channel::READ_EVENT);
		}
		if(events & EPOLLOUT){
			ch->SetReadyEvents(Channel::WRITE_EVENT);
		}
		if(events & EPOLLET){
			ch->SetReadyEvents(Channel::ET);
		}
		active_channels.push_back(ch);
	}
	return active_channels;
}

RC Epoll::UpdateChannel(Channel *ch)
{
	int sockfd = ch->fd();
	struct epoll_event ev{};
	ev.data.ptr = ch;
	if(ch->GetListenEvents() & Channel::READ_EVENT) {
		ev.events |= EPOLLIN | EPOLLPRI;
	}
	if(ch->GetListenEvents() & Channel::WRITE_EVENT) {
		ev.events |= EPOLLOUT;
	}
	if(ch->GetListenEvents() & Channel::ET) {
		ev.events |= EPOLLET;
	}
	if(!ch->GetExist()) {
		if(epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev) == -1){
			perror("epoll add error");
			return RC_EPOLL_ERROR;
		}
		ch->SetExist();
	} else {
		if(epoll_ctl(epfd_, EPOLL_CTL_MOD, sockfd, &ev) == -1){
			perror("epoll mod error");
			return RC_EPOLL_ERROR;
		}
	}
	return RC_SUCCESS;
}

RC Epoll::DeleteChannel(Channel *ch)
{
	int sockfd = ch->fd();
	if(epoll_ctl(epfd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1){
		perror("epoll delete error");
		return RC_EPOLL_ERROR;
	}
	ch->SetExist(false);
	return RC_SUCCESS;
}

