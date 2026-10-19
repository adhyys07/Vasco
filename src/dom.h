#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

enum class NodeType { Document, Element, Text };

struct Node {
	NodeType type = NodeType::Element;
    std::string tag;   
    std::string text;  
    std::map<std::string, std::string> attrs;
    std::vector<std::unique_ptr<Node>> children;
    Node* parent = nullptr;
 
    Node* add_child(std::unique_ptr<Node> child) {
        child->parent = this;
        children.push_back(std::move(child));
        return children.back().get();
    }

    const std::string& attr(const std::string& key) const {
        static const std::string empty;
        auto it = attrs.find(key);
        return it == attrs.end() ? empty : it->second;
    }
 
    static std::unique_ptr<Node> make_element(const std::string& tag) {
        auto n = std::make_unique<Node>();
        n->type = NodeType::Element;
        n->tag = tag;
        return n;
    }
 
    static std::unique_ptr<Node> make_text(const std::string& text) {
        auto n = std::make_unique<Node>();
        n->type = NodeType::Text;
        n->text = text;
        return n;
    }
};

const Node* find_first(const Node* root, const std::string& tag);
std::string text_content(const Node* node);
void dump_tree(const Node* node, int depth = 0);