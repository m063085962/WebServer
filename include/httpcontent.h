#pragma once

#include <utility>
#include <algorithm>

#include "buffer.h"
#include "httprequest.h"
#include "httpparsestate.h"

class HttpContent {
public:
	HttpContent();
	~HttpContent();

	bool ParseContent(Buffer* buffer);
	bool GetCompleteRequest() { return parse_state_ == kParseGotCompleteRequest; }

	const HttpRequest& request() const { return request_; }
	void ResetContentState() {
		HttpRequest tmp;
		request_.Swap(tmp);
		parse_state_ = kParseRequestLine;
	}

private:
	DISALLOW_COPY_AND_MOVE(HttpContent);

	HttpRequest request_;
	HttpRequestParseState parse_state_;
};

