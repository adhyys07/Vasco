#pragma once
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "dom.h"
#include "style.h"

struct FontKey = {
	int px = 16;
	bool bold = false;
	bool italic = false;
	bool mono = false;

	bool operator<(const FontKey & o) const {
        return std::tie(px, bold, italic, mono) < std::tie(o.px, o.bold, o.italic, o.mono);
	}
};

struct TextRun {
	int x = 0, y =0, w = 0, h = 0;
	std::string text;
    FontKey font;
    uint32_t color = 0x000000;
    bool underline = false;
    std::string href;
};
 
struct RectCmd {
    int x = 0, y = 0, w = 0, h = 0;
    uint32_t color = 0x000000;
};

struct DisplayList {
    std::vector<RectCmd> rects;
    std::vector<TextRun> texts;
    int height = 0;
};
 

class FontMetrics {
public:
    virtual ~FontMetrics() = default;
    virtual int measure(const std::string& utf8, const FontKey& font) = 0;
    virtual int line_height(const FontKey& font) = 0;
    virtual int ascent(const FontKey& font) = 0;  
    constexpr int kBaseFontPx = 16;
 
DisplayList layout_document(const Node* document, int viewport_width, FontMetrics& metrics);