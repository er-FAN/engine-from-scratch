# =====================================================================
# FindSDL3Local.cmake
# This file locates SDL3 from the user's own machine.
# Anyone who downloads this repository must set up their SDL3 path once
# (only once, not per stage) using one of the methods below:
#
# Option 1 (recommended - persistent): define an Environment Variable named SDL3_ROOT
#   Windows (PowerShell):  setx SDL3_ROOT "C:\Libs\SDL3"
#   Windows (CMD):         setx SDL3_ROOT "C:\Libs\SDL3"
#   Linux/macOS (bash):    export SDL3_ROOT=/home/user/libs/SDL3
#
# Option 2 (temporary - for a single build): pass it directly when running cmake
#   cmake -S . -B build -DSDL3_ROOT="C:/Libs/SDL3"
#
# SDL3_ROOT must point to a folder that contains SDL3's include/ and lib/
# (and on Windows, bin/) subfolders. That's the same folder you get from
# the Prebuilt download, or from `cmake --install`.
# =====================================================================

# If the user didn't pass it directly via -D, read it from the Environment Variable
if(NOT SDL3_ROOT)
    if(DEFINED ENV{SDL3_ROOT})
        set(SDL3_ROOT "$ENV{SDL3_ROOT}")
    endif()
endif()

if(NOT SDL3_ROOT)
    message(FATAL_ERROR
        "\n"
        "SDL3 path not found!\n"
        "Please do one of the following:\n"
        "  1) Create an Environment Variable named SDL3_ROOT pointing to the SDL3 folder\n"
        "  2) Or pass it when running cmake: -DSDL3_ROOT=\"path/to/SDL3\"\n"
    )
endif()

if(NOT EXISTS "${SDL3_ROOT}/include/SDL3/SDL.h")
    message(FATAL_ERROR "SDL3_ROOT doesn't look correct: ${SDL3_ROOT}\n(include/SDL3/SDL.h not found)")
endif()

# Detect the target architecture (x64 / x86 / arm64), since some SDL3
# distributions keep a separate subfolder per architecture inside lib/
if(CMAKE_GENERATOR_PLATFORM)
    set(SDL3_ARCH ${CMAKE_GENERATOR_PLATFORM})
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDL3_ARCH "x64")
else()
    set(SDL3_ARCH "x86")
endif()

set(SDL3_LIB_DIR "${SDL3_ROOT}/lib/${SDL3_ARCH}")

if(NOT EXISTS "${SDL3_LIB_DIR}/SDL3.lib")
    message(FATAL_ERROR
        "SDL3.lib not found at this path: ${SDL3_LIB_DIR}\n"
        "Check the subfolders available under lib (x64 / x86 / arm64) and make sure one matches your build architecture."
    )
endif()

message(STATUS "Using SDL3 (${SDL3_ARCH}) from: ${SDL3_LIB_DIR}")

# Build an interface target so any project can link it easily
add_library(SDL3Local INTERFACE)
target_include_directories(SDL3Local INTERFACE "${SDL3_ROOT}/include")
target_link_directories(SDL3Local INTERFACE "${SDL3_LIB_DIR}")
target_link_libraries(SDL3Local INTERFACE SDL3)

# Keep the dll path around so it can be copied next to the exe after building (Windows only)
set(SDL3_DLL_PATH "${SDL3_LIB_DIR}/SDL3.dll" CACHE INTERNAL "SDL3 dll path")

# Helper function: copies the dll next to the exe output after building (Windows only)
function(sdl3_copy_dll target_name)
    if(WIN32 AND EXISTS "${SDL3_DLL_PATH}")
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_DLL_PATH}"
                "$<TARGET_FILE_DIR:${target_name}>"
        )
    endif()
endfunction()