#pragma once

class TcpServer;
class EventLoop;
class Epoll;
class Acceptor;
class Connection;
class Channel;
class Socket;
class Buffer;
class ThreadPool;

#define DISALLOW_COPY(cname)     \
  cname(const cname &) = delete; \
  cname &operator=(const cname &) = delete;

#define DISALLOW_MOVE(cname) \
  cname(cname &&) = delete;  \
  cname &operator=(cname &&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname);               \
  DISALLOW_MOVE(cname);

enum RC
{
	RC_UNDEFINED,
	RC_SUCCESS,
	RC_SOCKET_ERROR,
	RC_EPOLL_ERROR,
	RC_CONNECTION_ERROR,
	RC_ACCEPTOR_ERROR,
	RC_UNIMPLEMENTED
};

