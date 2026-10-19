#pragma once
#include <string>

struct Url {
    std::string scheme = "http";
    std::string host;
    int port = 80;
    std::string path = "/";
 
    std::string to_string() const;
};
 
bool parse_url(const std::string& raw, Url& out, std::string& err);
 
bool resolve_url(const Url& base, const std::string& href, Url& out, std::string& err);