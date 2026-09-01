# =====================================================================
# FindSDL3ImageLocal.cmake
# این فایل باید بعد از FindSDL3Local.cmake include شود (چون به SDL3_ROOT نیاز دارد)
#
# پیش‌فرض: پوشه SDL3_image هم‌سطح (کنار) پوشه SDL3_ROOT قرار دارد. یعنی اگر:
#   SDL3_ROOT = C:\Libs\SDL3
# آنگاه به‌صورت خودکار این مسیر را امتحان می‌کند:
#   C:\Libs\SDL3_image
#
# اگر پوشه SDL3_image جای دیگری است، می‌توانی مستقیم مشخصش کنی:
#   Environment Variable:  SDL3_IMAGE_ROOT
#   یا موقع اجرای cmake:   -DSDL3_IMAGE_ROOT="مسیر/پوشه/SDL3_image"
# =====================================================================

if(NOT SDL3_ROOT)
    message(FATAL_ERROR "FindSDL3ImageLocal.cmake باید بعد از FindSDL3Local.cmake include شود")
endif()

# اگر کاربر صریح مسیر نداده، اول Environment Variable را چک کن
if(NOT SDL3_IMAGE_ROOT)
    if(DEFINED ENV{SDL3_IMAGE_ROOT})
        set(SDL3_IMAGE_ROOT "$ENV{SDL3_IMAGE_ROOT}")
    endif()
endif()

# اگر هنوز مشخص نشده، فرض کن کنار SDL3_ROOT است (هم‌سطح آن)
if(NOT SDL3_IMAGE_ROOT)
    get_filename_component(SDL3_PARENT_DIR "${SDL3_ROOT}" DIRECTORY)
    set(SDL3_IMAGE_ROOT "${SDL3_PARENT_DIR}/SDL3_image")
endif()

if(NOT EXISTS "${SDL3_IMAGE_ROOT}/include/SDL3_image/SDL_image.h")
    message(FATAL_ERROR
        "\n"
        "SDL3_image در این مسیر پیدا نشد: ${SDL3_IMAGE_ROOT}\n"
        "اگر پوشه‌اش جای دیگری است، این را تنظیم کن:\n"
        "  Environment Variable با نام SDL3_IMAGE_ROOT\n"
        "  یا: -DSDL3_IMAGE_ROOT=\"مسیر/پوشه/SDL3_image\"\n"
    )
endif()

# همان تشخیص معماری که برای SDL3 استفاده کردیم
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
        "SDL3_image.lib در این مسیر پیدا نشد: ${SDL3_IMAGE_LIB_DIR}\n"
        "زیرپوشه‌های موجود در lib را چک کن (x64 / x86 / arm64)."
    )
endif()

message(STATUS "SDL3_image (${SDL3_IMAGE_ARCH}) از این مسیر استفاده می‌شود: ${SDL3_IMAGE_LIB_DIR}")

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