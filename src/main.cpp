#include <SDL.h>

#include <cstdio>
#include <vector>

#include "dom/dom.h"
#include "dom/html.h"
#include "layout/layout.h"
#include "layout/style.h"
#include "net/http.h"
#include "net/url.h"
#include "ui/render.h"
 
namespace {

const char* kDefaultUrl = "http://info.cern.ch/hypertext/WWW/TheProject.html";

struct Browser {
	Renderer renderer;
	std::unique_ptr<Node> document;
	DisplayList display_list;
    Url current;
    std::vector<Url> history;
    int scroll = 0;
    bool needs_paint = true;

    void load(const Url& url, bool push_history) {
        HttpResponse response;
        Url final_url;
        std::string err;

        if (!http_get(url, response, final_url, err)) {
            std::printf("load failed: %s\n", err.c_str());
            response.body = "<html><body><h1>Could not load page</h1><p>" + err +
                            "</p></body></html>";
            final_url = url;
        }

        if (push_history && !current.host.empty()) history.push_back(current);
        current = final_url;

        document = parse_html(response.body);
        const Node* title = find_first(document.get(), "title");
        std::string title_text = title ? text_content(title) : current.to_string();
        if (title_text.empty()) title_text = "minibrowser";
        renderer.set_title(title_text + " - minibrowser");
        scroll = 0;
        relayout();
     }


    void relayout() {
        int w = 0, h = 0;
        renderer.window_size(w,h);
        if (w <= 0) w = 1024;
        display_list = layout_document(document.get(), w, renderer);
        clamp_scroll();
        needs_paint = true;
    }

    void clamp_scroll() {
        int w = 0, h = 0;
        renderer.window_size(w, h);
        int max_scroll = display_list.height - h;
        if (max_scroll < 0) max_scroll = 0;
        if (scroll > max_scroll) scroll = max_scroll;
        if (scroll < 0) scroll = 0;
    }
 
    void scroll_by(int delta) {
        int before = scroll;
        scroll += delta;
        clamp_scroll();
        if (scroll != before) needs_paint = true;
    }

    void click(int mouse_x, int mouse_y) {
        int doc_y = mouse_y + scroll;
        for (const TextRun& run : display_list.texts) {
            if (run.href.empty()) continue;
            if (mouse_x >= run.x && mouse_x < run.x + run.w && doc_y >= run.y &&
                doc_y < run.y + run.h) {
                Url target;
                std::string err;
                if (resolve_url(current, run.href, target, err)) {
                    std::printf("-> %s\n", target.to_string().c_str());
                    load(target, true);
                } else {
                    std::printf("bad link %s: %s\n", run.href.c_str(), err.c_str());
                }
                return;
            }
        }
    }

    void back() {
        if (history.empty()) return;
        Url previous = history.back();
        history.pop_back();
        load(previous, false);
    }
};

} 

int main(int argc, char* argv[]) {
    std::string url_text = kDefaultUrl;
    bool dump_only = false;
 
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--dump") {
            dump_only = true;
        } else if (!arg.empty() && arg[0] != '-') {
            url_text = arg;
        }
    }
 
    Url url;
    std::string err;
    if (!parse_url(url_text, url, err)) {
        std::printf("note: %s\n", err.c_str());
    }

    if (dump_only) {
        HttpResponse response;
        Url final_url;
        std::string fetch_err;
        if (!http_get(url, response, final_url, fetch_err)) {
            std::printf("fetch failed: %s\n", fetch_err.c_str());
            return 1;
        }
        std::printf("%d, %zu bytes\n\n", response.status, response.body.size());
        auto document = parse_html(response.body);
        dump_tree(document.get());
        if (document->children.empty()) {
            std::printf("\n(empty tree: parse_html is still a stub)\n");
        }
        return 0;
    }

    Browser browser;
    std::string init_err;
    if (!browser.renderer.init(1024, 768, init_err)) {
        std::printf("renderer unavailable: %s\n", init_err.c_str());
        std::printf("falling back to --dump so you can work on the parser first.\n\n");
        HttpResponse response;
        Url final_url;
        std::string fetch_err;
        if (http_get(url, response, final_url, fetch_err)) {
            auto document = parse_html(response.body);
            dump_tree(document.get());
            if (document->children.empty()) {
                std::printf("(empty tree: parse_html is still a stub)\n");
            }
        }
        return 0;
    }
 
    browser.load(url, false);
 
    bool running = true;
    while (running) {
        SDL_Event event;
        if (!SDL_WaitEvent(&event)) break;
 
        do {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
 
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
                        event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        browser.relayout();  
                    }
                    break;
 
                case SDL_MOUSEWHEEL:
                    browser.scroll_by(-event.wheel.y * 60);
                    break;
 
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        browser.click(event.button.x, event.button.y);
                    }
                    break;
                
                case SDL_KEYDOWN: {
                    int page = 0, win_h = 0, win_w = 0;
                    browser.renderer.window_size(win_w, win_h);
                    page = win_h > 100 ? win_h - 60 : 400;
 
                    switch (event.key.keysym.sym) {
                        case SDLK_DOWN: browser.scroll_by(60); break;
                        case SDLK_UP: browser.scroll_by(-60); break;
                        case SDLK_PAGEDOWN: case SDLK_SPACE: browser.scroll_by(page); break;
                        case SDLK_PAGEUP: browser.scroll_by(-page); break;
                        case SDLK_HOME: browser.scroll_by(-browser.display_list.height); break;
                        case SDLK_END: browser.scroll_by(browser.display_list.height); break;
                        case SDLK_BACKSPACE: browser.back(); break;
                        case SDLK_r: browser.load(browser.current, false); break;
                        case SDLK_q: case SDLK_ESCAPE: running = false; break;
                        default: break;
                    }
                    break;
                }
 
                default:
                    break;
            }
        } while (SDL_PollEvent(&event));
 
        if (running && browser.needs_paint) {
            browser.renderer.draw(browser.display_list, browser.scroll);
            browser.needs_paint = false;
        }
    }
 
    browser.renderer.shutdown();
    return 0;
}
