cmake_minimum_required(VERSION 3.16)
project(minibrowser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(minibrowser
  src/main.cpp
  src/url.cpp
  src/http.cpp
  src/dom.cpp
  src/html.cpp
  src/style.cpp
  src/layout.cpp
  src/render.cpp
)

targer_include_directories(minibrowser PRIVATE src)

find_package(SDL2 REQUIRED)
if(TARGET SDL2::SDL2)
	target_link_libraries(minibrowser PRIVATE SDL2_ttf::SDL2_ttf)
else()
	find_package(PkgConfig QUIET)
	if(PkgConfig_FOUND)
		pkg_check_modules(SDL2TTF QUIET SDL2_ttf)
  endif()
  if(SDL2TTF_FOUND)
    target_include_directories(minibrowser PRIVATE ${SDL2TTF_INCLUDE_DIRS})
    target_link_libraries(minibrowser PRIVATE ${SDL2TTF_LIBRARIES})
  else()
    find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf SDL2_ttf-2.0 REQUIRED)
    find_path(SDL2_TTF_INCLUDE_DIR NAMES SDL_ttf.h PATH_SUFFIXES SDL2 REQUIRED)
    target_include_directories(minibrowser PRIVATE ${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(minibrowser PRIVATE ${SDL2_TTF_LIBRARY})
  endif()
endif()

if(WIN32)
    target_link_libraries(minibrowser PRIVATE ws2_32)
endif()

if(MSVC)
    target_link_libraries(minibrowser ws2_32)
else()
    target_compile_options(minibrowser PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
