#pragma once
#include <memory>
#include <string>
#include "common.h"

class Buffer
{
public:
	Buffer() = default;
	~Buffer() = default;
	
	const std::string &buf() const;
	const char* c_str() const;
	void set_buf(const char *buf);

	void Append(const char* _str, int _size);
	ssize_t Size() const;
	void Clear();

private:
	std::string buf_;
};
