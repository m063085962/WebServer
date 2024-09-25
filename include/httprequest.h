#pragma once

#include "httpparsestate.h"

#include <string>
#include <map>

#include "common.h"

using std::string;

static const char http[] = "HTTP/1.";

enum Method {
	kGet,
	kPost,
	kPut,
	kDelete,
	kTrace,
	kOptions,
	kConnect,
	kPath
};

enum Version {
	kUnknown,
	kHttp10,
	kHttp11,
};

class HttpRequest {
public:
	HttpRequest();
	~HttpRequest();

	bool ParseRequestMethod(const char* start, const char* end);
	bool ParseRequestLine(const char* start, const char* end);
	bool ParseHeaders(const char* start, const char* colon, const char* end);
	bool ParseBody(const char* start, const char* end);

	Method method() const { return method_; }
	Version version() const { return version_; }
	const string& path() const { return path_; }
	const string& query() const { return query_; }
	const std::map<string, string>& headers() const { return headers_;}

	void Swap(HttpRequest& req);

	string GetHeader(const string& header) const {
		string ret;
		auto iter = headers_.find(header);
		return iter == headers_.end() ? ret : iter->second;
	}

private:
	DISALLOW_COPY_AND_MOVE(HttpRequest);

	Method method_;
	Version version_;
	string path_;
	string query_;
	std::map<string, string> headers_;
};

