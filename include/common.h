#pragma once

class TcpServer;
class EventLoop;
class Acceptor;
class Connection;
class Channel;
class Epoll;
class Socket;
class Buffer;
class ThreadPool;

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

