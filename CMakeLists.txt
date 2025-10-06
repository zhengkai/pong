cmake_minimum_required(VERSION 4.0.0)

project(pong VERSION 1.2)
set(PROJ pong)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MAKE_PROGRAM_ARGS "--no-print-directory")
set(VCPKG_VERBOSE OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/utf-8)
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
    src/sys/ver.cpp
    src/sys/debug.cpp
    src/sdl.cpp
    src/input.cpp
    src/event.cpp
    ${PROTO_SRC}
    ${PROTO_HDR}
)

target_link_libraries(${PROJ} PRIVATE
    box2d::box2d
    fmt::fmt
    protobuf::libprotobuf
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    spdlog::spdlog
    ${Protobuf_LIBRARIES}
)

target_include_directories(${PROJ} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_BINARY_DIR}/generated
)
