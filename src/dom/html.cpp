#include "dom/html.h"

std::unique_ptr<Node> parse_html(const std::string& source) {
    (void)source;
    auto doc = std::make_unique<Node>();
    doc->type = NodeType::Document;
    return doc;
}

std::string decode_entities(const std::string& input) {
    return input;
}