#include "render.h"

#include <SDL.h>
#include <SDL_ttf.h>

struct Renderer::Impl {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

Renderer::Renderer() : impl_(new Impl) {}

Renderer::~Renderer() {
    shutdown();
    delete impl_;
}

bool Renderer::init(int width, int height, std::string& err) {
    (void)width;
    (void)height;
    err = "Renderer::init not implemented yet";
    return false;
}

void Renderer::shutdown() {
}

int Renderer::measure(const std::string& utf8, const FontKey& font) {
    (void)utf8;
    (void)font;
    return 0;
}

int Renderer::line_height(const FontKey& font) {
    (void)font;
    return 0;
}

int Renderer::ascent(const FontKey& font) {
    (void)font;
    return 0;
}

void Renderer::draw(const DisplayList& list, int scroll_y) {
    (void)list;
    (void)scroll_y;
}

void Renderer::set_title(const std::string& title) {
    (void)title;
}

void Renderer::window_size(int& width, int& height) const {
    width = 0;
    height = 0;
}