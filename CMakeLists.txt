cmake_minimum_required(VERSION 3.20)

project(Chip8Emu VERSION 1.0 DESCRIPTION "A Chip-8 Emulator Written in C++")

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

set(SRC_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/interpreter.cpp
    ${SRC_DIR}/keyboard.cpp
    )

set(TARGET_NAME chip8emu)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE SDL3::SDL3)

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

