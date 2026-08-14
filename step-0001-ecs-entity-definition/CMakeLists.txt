cmake_minimum_required(VERSION 3.20)

# Define project name, and language
project(ecs_entity_definition LANGUAGES CXX)

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include header search directories
include_directories(include)

# Gather all C++ source files recursively within the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define target executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set compiler warning levels for better code quality
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()