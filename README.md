# Vasco

Vasco is a web browser built from scratch in C++17, with SDL2 for the window and
SDL2_ttf for text. It is a learning project: the module skeleton is in place and
the pipeline stages are being filled in one at a time.

## Layout

Sources are grouped by pipeline stage. Data flows top to bottom:

| Directory      | Role                                                         |
| -------------- | ------------------------------------------------------------ |
| `src/net/`     | `url` parsing/resolution, `http` fetching                    |
| `src/dom/`     | `dom` node tree, `html` tokenizer and tree builder           |
| `src/layout/`  | `style` resolution, `layout` box/line building               |
| `src/ui/`      | `render` — SDL2 window, font metrics, display-list painting   |
| `src/main.cpp` | CLI argument handling and the browser event loop             |

Project headers are included relative to the `src` root, so the path reads the
same from any file:

```cpp
#include "net/url.h"
#include "dom/html.h"
#include "layout/layout.h"
```

## Dependencies

SDL2 and SDL2_ttf. On Windows via MSYS2 (MINGW64 shell):

```sh
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake \
                   mingw-w64-x86_64-ninja mingw-w64-x86_64-pkgconf \
                   mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
```

CMake locates both through `pkg-config`, falling back to `find_package`.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run

```sh
./build/minibrowser.exe                 # default page, opens a window
./build/minibrowser.exe <url>           # load a specific url
./build/minibrowser.exe --dump          # fetch and print the DOM, no window
./build/minibrowser.exe <url> --dump
```

The default page is `http://info.cern.ch/hypertext/WWW/TheProject.html`.

| Input                  | Action              |
| ---------------------- | ------------------- |
| up / down              | scroll by a line    |
| page up / page down    | scroll by a page    |
| space                  | scroll down a page  |
| home / end             | jump to top/bottom  |
| mouse wheel            | scroll              |
| left click on a link   | follow it           |
| backspace              | go back             |
| `r`                    | reload              |
| `q` / escape           | quit                |

## Status

Scaffolding compiles and links; most pipeline stages are still stubs.

- [ ] `net/url` — `parse_url` and `resolve_url` report "not implemented"
- [ ] `net/http` — no sockets yet; `http_get` always returns HTTP 200 with a
      test page compiled into the binary, so the rest of the pipeline can be
      debugged offline. `max_redirects` is ignored.
- [ ] `dom/dom` — `find_first` and `text_content` are stubs; `dump_tree` works
- [ ] `dom/html` — `parse_html` returns an empty tree
- [ ] `layout/style` — style resolution
- [ ] `layout/layout` — `layout_document` returns an empty display list
- [ ] `ui/render` — `Renderer::init` and painting

So `--dump` currently prints `#document` with no children: the 865 bytes it
reports are the built-in page, and `parse_html` discards them.
