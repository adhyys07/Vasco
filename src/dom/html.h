#pragma once
#include <memory>
#include <string>

#include "dom/dom.h"

std::unique_ptr<Node> parse_html(const std::string& source);
 
std::string decode_entities(const std::string& input);