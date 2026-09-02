# =====================================================================
# FindSDL3ImageLocal.cmake
# This file must be included after FindSDL3Local.cmake (it depends on SDL3_ROOT)
#
# By default, this assumes the SDL3_image folder sits next to (at the same
# level as) SDL3_ROOT. That is, if:
#   SDL3_ROOT = C:\Libs\SDL3
# it will automatically try this path:
#   C:\Libs\SDL3_image
#
# If your SDL3_image folder is somewhere else, you can point to it directly:
#   Environment Variable:  SDL3_IMAGE_ROOT
#   or when running cmake: -DSDL3_IMAGE_ROOT="path/to/SDL3_image"
# =====================================================================

if(NOT SDL3_ROOT)
    message(FATAL_ERROR "FindSDL3ImageLocal.cmake must be included after FindSDL3Local.cmake")
endif()

# If the user didn't pass a path explicitly, check the Environment Variable first
if(NOT SDL3_IMAGE_ROOT)
    if(DEFINED ENV{SDL3_IMAGE_ROOT})
        set(SDL3_IMAGE_ROOT "$ENV{SDL3_IMAGE_ROOT}")
    endif()
endif()

# If still not set, assume it's next to SDL3_ROOT (same parent folder)
if(NOT SDL3_IMAGE_ROOT)
    get_filename_component(SDL3_PARENT_DIR "${SDL3_ROOT}" DIRECTORY)
    set(SDL3_IMAGE_ROOT "${SDL3_PARENT_DIR}/SDL3_image")
endif()

if(NOT EXISTS "${SDL3_IMAGE_ROOT}/include/SDL3_image/SDL_image.h")
    message(FATAL_ERROR
        "\n"
        "SDL3_image not found at this path: ${SDL3_IMAGE_ROOT}\n"
        "If it's located elsewhere, set one of these:\n"
        "  An Environment Variable named SDL3_IMAGE_ROOT\n"
        "  Or: -DSDL3_IMAGE_ROOT=\"path/to/SDL3_image\"\n"
    )
endif()

# Same architecture detection logic used for SDL3
if(CMAKE_GENERATOR_PLATFORM)
    set(SDL3_IMAGE_ARCH ${CMAKE_GENERATOR_PLATFORM})
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL3_IMAGE_ARCH "x64")
else()
    set(SDL3_IMAGE_ARCH "x86")
endif()

set(SDL3_IMAGE_LIB_DIR "${SDL3_IMAGE_ROOT}/lib/${SDL3_IMAGE_ARCH}")

if(NOT EXISTS "${SDL3_IMAGE_LIB_DIR}/SDL3_image.lib")
    message(FATAL_ERROR
        "SDL3_image.lib not found at this path: ${SDL3_IMAGE_LIB_DIR}\n"
        "Check the subfolders available under lib (x64 / x86 / arm64)."
    )
endif()

message(STATUS "Using SDL3_image (${SDL3_IMAGE_ARCH}) from: ${SDL3_IMAGE_LIB_DIR}")

add_library(SDL3ImageLocal INTERFACE)
target_include_directories(SDL3ImageLocal INTERFACE "${SDL3_IMAGE_ROOT}/include")
target_link_directories(SDL3ImageLocal INTERFACE "${SDL3_IMAGE_LIB_DIR}")
target_link_libraries(SDL3ImageLocal INTERFACE SDL3_image SDL3Local)

set(SDL3_IMAGE_DLL_PATH "${SDL3_IMAGE_LIB_DIR}/SDL3_image.dll" CACHE INTERNAL "SDL3_image dll path")

function(sdl3_image_copy_dll target_name)
    if(WIN32 AND EXISTS "${SDL3_IMAGE_DLL_PATH}")
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_IMAGE_DLL_PATH}"
                "$<TARGET_FILE_DIR:${target_name}>"
        )
    endif()
endfunction()