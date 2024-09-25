#pragma once

#include <string.h>

#include <string>
#include <algorithm>

#include "common.h"

static const char* kCRLF = "\r\n";

class Buffer {
public:
	Buffer() : index_(0) {}
	~Buffer() = default;
	
	const std::string &buf() const;
	const char* c_str() const;
	size_t Size() const;

	void set_buf(const char* buf);
	void Clear();

	void Append(const char* str, int size) {
		for (int i=0; i < size; i++) {
			if (str[i] == '\0') break;
			buf_.push_back(str[i]);
		}
	}
	void Append(const char* str) {
		Append(str, static_cast<int>(strlen(str)));
	}
	void Append(const std::string& str) {
		Append(str.data(), static_cast<int>(str.size()));
	}

	const char* begin() const { return &*buf_.begin(); }

	const char* end() const { return &*buf_.end(); }

	const char* Peek() const { return begin() + index_; }

	const char* FindCRLF() const {
		const char* find = std::search(Peek(), end(), kCRLF, kCRLF + 2);
		return find == end() ? nullptr : find;
	}

	void RetrieveUntilIndex(const char* index) {
		index_ += static_cast<int>(index - begin());
	}
	
private:
	DISALLOW_COPY_AND_MOVE(Buffer);

	std::string buf_;
	int index_;
};

