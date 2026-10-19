cmake_minimum_required(VERSION 3.16)
project(minibrowser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Sources grouped by subsystem; the browser pipeline runs net -> dom -> layout -> ui.
add_executable(minibrowser
  src/main.cpp

  src/net/url.cpp
  src/net/http.cpp

  src/dom/dom.cpp
  src/dom/html.cpp

  src/layout/style.cpp
  src/layout/layout.cpp

  src/ui/render.cpp
)

# All project headers are included src-root-relative, e.g. #include "net/url.h".
target_include_directories(minibrowser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(SDL2PC QUIET sdl2)
  pkg_check_modules(TTFPC QUIET SDL2_ttf)
endif()

if(SDL2PC_FOUND AND TTFPC_FOUND)
  message(STATUS "SDL2 ${SDL2PC_VERSION} + SDL2_ttf ${TTFPC_VERSION} (pkg-config)")
  target_include_directories(minibrowser PRIVATE ${SDL2PC_INCLUDE_DIRS} ${TTFPC_INCLUDE_DIRS})
  target_link_directories(minibrowser PRIVATE ${SDL2PC_LIBRARY_DIRS} ${TTFPC_LIBRARY_DIRS})
  target_link_libraries(minibrowser PRIVATE ${SDL2PC_LIBRARIES} ${TTFPC_LIBRARIES})
else()
  message(STATUS "pkg-config did not find SDL2/SDL2_ttf, trying find_package")
  find_package(SDL2 REQUIRED)
  if(TARGET SDL2::SDL2)
    target_link_libraries(minibrowser PRIVATE SDL2::SDL2)
    if(TARGET SDL2::SDL2main)
      target_link_libraries(minibrowser PRIVATE SDL2::SDL2main)
    endif()
  else()
    target_include_directories(minibrowser PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(minibrowser PRIVATE ${SDL2_LIBRARIES})
  endif()

  find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf SDL2_ttf-2.0 REQUIRED)
  find_path(SDL2_TTF_INCLUDE_DIR NAMES SDL_ttf.h PATH_SUFFIXES SDL2 REQUIRED)
  target_include_directories(minibrowser PRIVATE ${SDL2_TTF_INCLUDE_DIR})
  target_link_libraries(minibrowser PRIVATE ${SDL2_TTF_LIBRARY})
endif()

if(WIN32)
  target_link_libraries(minibrowser PRIVATE ws2_32)
endif()

if(MSVC)
  target_compile_options(minibrowser PRIVATE /W4 /utf-8)
else()
  target_compile_options(minibrowser PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()