#include "net/url.h"

std::string Url::to_string() const {
	return "";
}

bool parse_url(const std::string& raw, Url& out, std::string& err) {
    err = "parse_url not implemented yet";
    return false;
}

bool resolve_url(const Url& base, const std::string& href, Url& out, std::string& err) {    
    err = "resolve_url not implemented yet";
    return false;
}

