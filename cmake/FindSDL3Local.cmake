# =====================================================================
# FindSDL3Local.cmake
# =====================================================================

if(NOT SDL3_ROOT)
    if(DEFINED ENV{SDL3_ROOT})
        set(SDL3_ROOT "$ENV{SDL3_ROOT}")
    endif()
endif()

if(NOT SDL3_ROOT)
    message(FATAL_ERROR
        "\n"
        "مسیر SDL3 پیدا نشد!\n"
        "لطفاً یکی از راه‌های زیر را انجام بده:\n"
        "  1) Environment Variable با نام SDL3_ROOT بساز و به پوشه SDL3 اشاره بده\n"
        "  2) یا موقع اجرای cmake بنویس: -DSDL3_ROOT=\"مسیر/پوشه/SDL3\"\n"
    )
endif()

if(NOT EXISTS "${SDL3_ROOT}/include/SDL3/SDL.h")
    message(FATAL_ERROR "مسیر SDL3_ROOT درست به نظر نمی‌رسد: ${SDL3_ROOT}")
endif()

# تشخیص معماری (x64 / x86 / arm64) چون این نسخه از SDL3 برای هر معماری
# یک زیرپوشه‌ی جدا داخل lib دارد
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
        "SDL3.lib در این مسیر پیدا نشد: ${SDL3_LIB_DIR}\n"
        "زیرپوشه‌های موجود در lib را چک کن (x64 / x86 / arm64) و مطمئن شو با معماری بیلدت یکی است."
    )
endif()

message(STATUS "SDL3 (${SDL3_ARCH}) از این مسیر استفاده می‌شود: ${SDL3_LIB_DIR}")

add_library(SDL3Local INTERFACE)
target_include_directories(SDL3Local INTERFACE "${SDL3_ROOT}/include")
target_link_directories(SDL3Local INTERFACE "${SDL3_LIB_DIR}")
target_link_libraries(SDL3Local INTERFACE SDL3)

set(SDL3_DLL_PATH "${SDL3_LIB_DIR}/SDL3.dll" CACHE INTERNAL "SDL3 dll path")

function(sdl3_copy_dll target_name)
    if(WIN32 AND EXISTS "${SDL3_DLL_PATH}")
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL3_DLL_PATH}"
                "$<TARGET_FILE_DIR:${target_name}>"
        )
    endif()
endfunction()