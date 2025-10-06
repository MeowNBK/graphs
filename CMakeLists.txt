cmake_minimum_required(VERSION 3.25)
project(meow-graphs VERSION 0.1.0 LANGUAGES CXX)

# --- Cài đặt chuẩn C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Đường dẫn output ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# --- Thu thập source files ---
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

# --- Xử lý PCH ---
set(PCH_HEADER "${PROJECT_SOURCE_DIR}/include/pch.h")
set(USE_PCH FALSE)
if(EXISTS "${PCH_HEADER}")
    set(USE_PCH TRUE)
endif()

foreach(SOURCE_FILE ${APP_SOURCES})
    get_filename_component(FILE_NAME_WITH_EXT ${SOURCE_FILE} NAME)
    get_filename_component(TARGET_NAME ${FILE_NAME_WITH_EXT} NAME_WE)

    add_executable(${TARGET_NAME} ${SOURCE_FILE})

    target_include_directories(${TARGET_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
    )

    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
    )

    if(USE_PCH)
        target_precompile_headers(${TARGET_NAME} PRIVATE "${PCH_HEADER}")
    endif()
endforeach()
