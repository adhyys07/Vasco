#pragma once
#include <string>

#include "layout/layout.h"

class Renderer : public FontMetrics {
public:
	Renderer();
	~Renderer() override;

	bool init(int width, int height, std::string& err);
	void shutdown();

	int measure(const std::string& utf8, const FontKey& font) override;
    int line_height(const FontKey& font) override;
    int ascent(const FontKey& font) override;
 
    void draw(const DisplayList& list, int scroll_y);
    void set_title(const std::string& title);
    void window_size(int& width, int& height) const;

private:
	struct Impl;
	Impl* impl_ = nullptr;
};