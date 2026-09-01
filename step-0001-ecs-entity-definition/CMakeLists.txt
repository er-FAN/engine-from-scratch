cmake_minimum_required(VERSION 3.20)

# Define project name, and language
project(ecs_entity_definition LANGUAGES CXX)

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Pull in SDL3 from the user's local machine (path via SDL3_ROOT env var or -D flag)
include(${CMAKE_SOURCE_DIR}/../cmake/FindSDL3Local.cmake)

# Pull in SDL3_image (path resolved automatically next to SDL3_ROOT, or via SDL3_IMAGE_ROOT)
include(${CMAKE_SOURCE_DIR}/../cmake/FindSDL3ImageLocal.cmake)

# Include header search directories (own headers + shared SimpleSDL library)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/../Common)

# Gather all C++ source files recursively within the src directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Add the shared SimpleSDL wrapper source
list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/../Common/SimpleSDL.cpp)

# Define target executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SDL3 and SDL3_image
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3Local SDL3ImageLocal)

# Copy SDL3.dll and SDL3_image.dll next to the executable after build (Windows only)
sdl3_copy_dll(${PROJECT_NAME})
sdl3_image_copy_dll(${PROJECT_NAME})

# Set compiler warning levels for better code quality
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)