#include "buffer.h"

#include <string>
#include <algorithm>

const std::string &Buffer::buf() const {return buf_;}

const char *Buffer::c_str() const {return buf_.c_str();}

size_t Buffer::Size() const { return buf_.size(); }

void Buffer::set_buf(const char *buf) {
	std::string new_buf(buf);
	buf_.swap(new_buf);
}

void Buffer::Clear() { buf_.clear(); }

