#pragma once
#include <cstdint>
#include <string>

struct Style {
    bool block = false;        
    bool hidden = false;       
    bool bold = false;
    bool italic = false;
    bool mono = false;
    bool underline = false;
    bool preformatted = false;  
    bool list_item = false;     
    bool rule = false;          
    float font_scale = 1.0f;    
    int margin_top = 0;
    int margin_bottom = 0;
    int indent = 0;            
    uint32_t color = 0x000000;  
};

Style style_for(const std::string& tag);

bool is_void_element(const std::string& tag);