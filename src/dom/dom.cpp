#include "dom/dom.h"
#include <cstdio>

const Node* find_first(const Node* root, const std::string& tag) {
	(void)root;
	(void)tag;
	return nullptr;
}

std::string text_content(const Node* node) {
    (void)node;
    return "";
}

void dump_tree(const Node* node, int depth) {
	if (!node) return;
    for (int i = 0; i < depth; i++) std::fputs("  ", stdout);

	switch (node->type) {
		case NodeType::Document:
			std::fputs("#document\n", stdout);
            break;
        case NodeType::Element: {
            std::printf("<%s", node->tag.c_str());
            for (const auto& kv : node->attrs) {
                std::printf(" %s=\"%s\"", kv.first.c_str(), kv.second.c_str());
            }
            std::fputs(">\n", stdout);
            break;
        }
        case NodeType::Text: {
            std::string t = node->text;
            if (t.size() > 60) t = t.substr(0, 57) + "...";
            for (char& c : t) {
                if (c == '\n' || c == '\r' || c == '\t') c = ' ';
            }
            std::printf("#text \"%s\"\n", t.c_str());
            break;
        }
    }
 
    for (const auto& child : node->children) dump_tree(child.get(), depth + 1);
}