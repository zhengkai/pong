cmake_minimum_required(VERSION 4.0.0)

project(pong VERSION 1.2)
set(PROJ pong)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM_ARGS "--no-print-directory")
set(VCPKG_VERBOSE OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_SHARED_LIBS OFF)  # 全局静态库

if (CMAKE_BUILD_TYPE STREQUAL "debug")
    target_compile_options(${PROJ} PRIVATE
        -Wall -Wextra -Werror
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
    target_link_options(${PROJ} PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -static-libasan
        -static-libubsan
    )
endif()

if (MSVC)
    add_compile_options(/utf-8)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # 静态 CRT
endif()

include(cmake/build-info.cmake)

find_package(box2d CONFIG REQUIRED QUIET)
find_package(fmt CONFIG REQUIRED QUIET)
find_package(Protobuf CONFIG REQUIRED QUIET)
find_package(spdlog CONFIG REQUIRED QUIET)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

set(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/proto/map.proto)

protobuf_generate_cpp(
    PROTO_SRC
    PROTO_HDR
    ${PROTO_FILE}
)

add_executable(${PROJ}
    src/main.cpp
    src/pong.cpp
    src/sdl.cpp
    src/input.cpp
    src/event.cpp
    src/render/text.cpp
    src/render/grid.cpp
    src/physics/physics.cpp
    ${PROTO_SRC}
    ${PROTO_HDR}
)

target_precompile_headers(${PROJ} PRIVATE
    <SDL3/SDL.h>
    <SDL3/SDL_events.h>
    <SDL3_image/SDL_image.h>
    <SDL3_ttf/SDL_ttf.h>
    <chrono>
    <cmath>
    <cstdio>
    <cstring>
    <fmt/format.h>
    <google/protobuf/stubs/common.h>
    <random>
    <spdlog/sinks/stdout_color_sinks.h>
    <spdlog/spdlog.h>
    <string>
    <thread>
    <vector>
)

target_link_libraries(${PROJ} PRIVATE
    box2d::box2d
    fmt::fmt
    protobuf::libprotobuf
    SDL3::SDL3-static
    SDL3_image::SDL3_image-static
    SDL3_ttf::SDL3_ttf-static
    spdlog::spdlog
    ${Protobuf_LIBRARIES}
)

target_include_directories(${PROJ} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/generated
)
