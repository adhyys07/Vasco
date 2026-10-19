#pragma once
#include <map>
#include <string>

#include "net/url.h"

struct HttpResponse {
	int status = 0;
	std::map<std::string, std::string> headers; 
    std::string body;
};

bool http_get(const Url& url, HttpResponse& out, Url& final_url, std::string& err,
              int max_redirects = 5);
 