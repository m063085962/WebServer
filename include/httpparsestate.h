#pragma once

enum HttpRequestParseState {
	kParseRequestLine,
	kParseHeaders,
	kParseBody,
	kParseGotCompleteRequest,
	kParseErrno,
};

