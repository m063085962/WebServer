#pragma once

#include <string>
#include "common.h"

class Buffer {
public:
	Buffer() = default;
	~Buffer() = default;
	
	const std::string &buf() const;
	const char* c_str() const;
	size_t Size() const;

	void set_buf(const char *buf);
	void Append(const char *_str, int _size);
	void Clear();

private:
	DISALLOW_COPY_AND_MOVE(Buffer);

	std::string buf_;
};
