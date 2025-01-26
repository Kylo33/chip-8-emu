cmake_minimum_required(VERSION 3.10)

project(Chip8Emu VERSION 1.0 DESCRIPTION "A Chip-8 Emulator Written in C++")

set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

set(SRC_FILES
    ${SRC_DIR}/main.cpp
    )

set(TARGET_NAME chip8emu)

add_executable(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE SDL3::SDL3)
