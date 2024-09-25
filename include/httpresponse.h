#pragma once

#include <string>
#include <utility>

#include "httprequest.h"
#include "common.h"

using std::string;

static const string CRLF = "\r\n";

enum HttpStatusCode {
	k100Continue = 100,
	k200OK = 200,
	k400BadRequest = 400,
	k403Forbidden = 403,
	k404NotFound = 404,
	k500InternalServerErrno = 500
};

class HttpResponse {
public:
	HttpResponse(bool close_connection) : type_("test/plain"),
										  close_connection_(close_connection) {
	}
	~HttpResponse() {}

	void SetStatusCode(HttpStatusCode status_code) { status_code_ = status_code; }
	void SetStatusMessage(const string& status_message) { status_message_ = std::move(status_message); }
	void SetCloseConnection(bool close_connection) { close_connection_ = close_connection; }
	
	void SetBodyType(const string& type) { type_ = type; }
	void SetBody(const string& body) { body_ = body; }
	void AppendToBuffer(Buffer* buffer);

	bool closeconnection() const { return close_connection_; }

private:
	DISALLOW_COPY_AND_MOVE(HttpResponse);

	static const string server_name_;
	HttpStatusCode status_code_;
	string status_message_;
	string body_;
	string type_;
	bool close_connection_;
};

