# =====================================================================
# FindImGuiLocal.cmake
# This file locates a raw Dear ImGui source checkout (as cloned from
# GitHub: https://github.com/ocornut/imgui) on the user's own machine,
# and builds it into a static library, linked against SDL3 + the
# SDL_Renderer backend (imgui_impl_sdl3 / imgui_impl_sdlrenderer3).
#
# IMPORTANT: This file must be include()'d AFTER FindSDL3Local.cmake,
# since it depends on the SDL3Local target and the SDL3_ROOT variable.
#
# By default, this script assumes the imgui folder sits right next to
# your SDL3 folder, e.g.:
#   C:/Libs/SDL3
#   C:/Libs/imgui
# so nothing needs to be configured if your layout matches that.
#
# If your imgui folder lives somewhere else, set IMGUI_ROOT the same
# way you set SDL3_ROOT:
#
# Option 1 (persistent): Environment Variable named IMGUI_ROOT
#   Windows (PowerShell):  setx IMGUI_ROOT "C:\Libs\imgui"
#   Linux/macOS (bash):    export IMGUI_ROOT=/home/user/libs/imgui
#
# Option 2 (temporary): pass it directly when running cmake
#   cmake -S . -B build -DIMGUI_ROOT="C:/Libs/imgui"
#
# IMGUI_ROOT must point to the root of the imgui repo itself (the
# folder that directly contains imgui.h, imgui.cpp, and a backends/
# subfolder) - NOT a build output or an include/lib style folder.
# =====================================================================

# Make sure SDL3 was set up first, since the SDL3 backend headers need it
if(NOT TARGET SDL3Local)
    message(FATAL_ERROR
        "\n"
        "FindImGuiLocal.cmake requires SDL3Local to already exist.\n"
        "Please include(FindSDL3Local.cmake) BEFORE including this file.\n"
    )
endif()

# If the user didn't pass IMGUI_ROOT directly via -D, read it from the Environment Variable
if(NOT IMGUI_ROOT)
    if(DEFINED ENV{IMGUI_ROOT})
        set(IMGUI_ROOT "$ENV{IMGUI_ROOT}")
    endif()
endif()

# If still not set, default to a folder named "imgui" sitting next to SDL3_ROOT
if(NOT IMGUI_ROOT)
    get_filename_component(SDL3_PARENT_DIR "${SDL3_ROOT}" DIRECTORY)
    set(IMGUI_ROOT "${SDL3_PARENT_DIR}/imgui")
    message(STATUS "IMGUI_ROOT not set, guessing default next to SDL3_ROOT: ${IMGUI_ROOT}")
endif()

if(NOT EXISTS "${IMGUI_ROOT}/imgui.h")
    message(FATAL_ERROR
        "\n"
        "imgui path not found or incorrect: ${IMGUI_ROOT}\n"
        "(imgui.h not found there)\n"
        "Please do one of the following:\n"
        "  1) Place the imgui folder right next to your SDL3 folder (same parent directory)\n"
        "  2) Create an Environment Variable named IMGUI_ROOT pointing to the imgui folder\n"
        "  3) Or pass it when running cmake: -DIMGUI_ROOT=\"path/to/imgui\"\n"
    )
endif()

set(IMGUI_BACKENDS_DIR "${IMGUI_ROOT}/backends")

if(NOT EXISTS "${IMGUI_BACKENDS_DIR}/imgui_impl_sdl3.h")
    message(FATAL_ERROR
        "SDL3 backend not found at: ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl3.h\n"
        "Make sure IMGUI_ROOT points at the full imgui repo (with its backends/ folder), not a partial copy."
    )
endif()

message(STATUS "Using ImGui from: ${IMGUI_ROOT}")

# Core ImGui sources (the library itself)
set(IMGUI_CORE_SOURCES
    "${IMGUI_ROOT}/imgui.cpp"
    "${IMGUI_ROOT}/imgui_draw.cpp"
    "${IMGUI_ROOT}/imgui_tables.cpp"
    "${IMGUI_ROOT}/imgui_widgets.cpp"
    "${IMGUI_ROOT}/imgui_demo.cpp"   # optional: remove this line if you don't want the demo window compiled in
    "${IMGUI_ROOT}/misc/cpp/imgui_stdlib.cpp"
)

# SDL3 + SDL_Renderer backend sources
set(IMGUI_BACKEND_SOURCES
    "${IMGUI_BACKENDS_DIR}/imgui_impl_sdl3.cpp"
    "${IMGUI_BACKENDS_DIR}/imgui_impl_sdlrenderer3.cpp"
)

# Build ImGui as a static library so it's compiled once and reused,
# instead of being recompiled per target that links it.
add_library(ImGuiLocal STATIC ${IMGUI_CORE_SOURCES} ${IMGUI_BACKEND_SOURCES})

target_include_directories(ImGuiLocal PUBLIC
    "${IMGUI_ROOT}"
    "${IMGUI_BACKENDS_DIR}"
    "${IMGUI_ROOT}/misc/cpp"
)

# The SDL3 backend headers include SDL3/SDL.h, so ImGuiLocal needs
# SDL3's include/lib paths too. PUBLIC so anything that links ImGuiLocal
# also automatically gets SDL3.
target_link_libraries(ImGuiLocal PUBLIC SDL3Local)