#pragma once
#include <string>

class Buffer
{
private:
	std::string buf_;
public:
	Buffer() = default;
	~Buffer() = default;

	void Append(const char* _str, int _size);
	ssize_t Size();
	const char *ToStr();
	void Clear();
	void Getline();
	void SetBuf(const char *buf);
};
