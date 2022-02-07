find_package(CURL REQUIRED)
find_package(ICU OPTIONAL_COMPONENTS i18n)
find_package(ICU OPTIONAL_COMPONENTS uc)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)

pkg_search_module(LIBUV libuv REQUIRED)

target_sources(
    mbgl-core
    PRIVATE
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/storage/http_file_source.cpp


        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/i18n/collator.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/i18n/number_format.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/text/local_glyph_rasterizer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/async_task.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/compression.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/image.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/jpeg_reader.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/logging_stderr.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/png_reader.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/png_writer.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/run_loop.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/string_stdlib.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/thread.cpp
        ${MBGL_SOURCE_DIR}/platform/default/src/mbgl/util/timer.cpp
        ${MBGL_SOURCE_DIR}/platform/linux/src/gl_functions.cpp
)

# FIXME: Should not be needed, but now needed by node because of the headless frontend.
target_include_directories(
    mbgl-core
    PUBLIC ${MBGL_SOURCE_DIR}/platform/default/include
    PRIVATE
        ${CURL_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
        ${LIBUV_INCLUDE_DIRS}
        ${X11_INCLUDE_DIRS}
)

# Always use vendored ICU
# TODO: verify if ICU has been replaced / removed
# include(${MBGL_SOURCE_DIR}/vendor/icu.cmake)
include(${MBGL_SOURCE_DIR}/vendor/nunicode.cmake)
include(${MBGL_SOURCE_DIR}/vendor/sqlite.cmake)

if(NOT ${ICU_FOUND} OR "${ICU_VERSION}" VERSION_LESS 62.0)
    message(STATUS "ICU not found or too old, using builtin.")

    set(MBGL_USE_BUILTIN_ICU TRUE)
    include(${PROJECT_SOURCE_DIR}/vendor/icu.cmake)

    set_source_files_properties(
        ${PROJECT_SOURCE_DIR}/platform/default/src/mbgl/i18n/number_format.cpp
        PROPERTIES
        COMPILE_DEFINITIONS
        MBGL_USE_BUILTIN_ICU
    )
endif()


if(MBGL_WITH_EGL)
    message(STATUS "Using EGL backend")
    find_package(OpenGL REQUIRED EGL)
    target_sources(
        mbgl-core
        PRIVATE
            ${MBGL_SOURCE_DIR}/platform/linux/src/headless_backend_egl.cpp
    )
    target_link_libraries(
        mbgl-core
        PRIVATE
            OpenGL::EGL
    )
else()
message(STATUS "Using GLX backend")
    find_package(OpenGL REQUIRED GLX)
    target_sources(
        mbgl-core
        PRIVATE
            ${MBGL_SOURCE_DIR}/platform/linux/src/headless_backend_glx.cpp
    )
    target_link_libraries(
        mbgl-core
        PRIVATE
            OpenGL::GLX
    )
endif()



target_link_libraries(
    mbgl-core
    PRIVATE
        ${CURL_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${LIBUV_LIBRARIES}
        ${X11_LIBRARIES}
        $<$<NOT:$<BOOL:${MBGL_USE_BUILTIN_ICU}>>:ICU::i18n>
        $<$<NOT:$<BOOL:${MBGL_USE_BUILTIN_ICU}>>:ICU::uc>
        $<$<BOOL:${MBGL_USE_BUILTIN_ICU}>:mbgl-vendor-icu>
        PNG::PNG
        # mbgl-vendor-icu
        mbgl-vendor-nunicode
        mbgl-vendor-sqlite
)
