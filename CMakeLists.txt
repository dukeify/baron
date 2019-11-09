cmake_minimum_required(VERSION 3.8)

project(baron VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

if(${BUILD_BARON_DEBUG})
    set(CMAKE_BUILD_TYPE Debug)
    set(compiler_flags "\
        -g\
        -fstandalone-debug\
        -O0\
        -fno-inline\
        -fno-omit-frame-pointer\
        -DBARON_DEBUG\
    ")
else()
    set(CMAKE_BUILD_TYPE Release)
    set(compiler_flags "-O3")
endif()

if(${BUILD_BARON_ASAN})
    set(compiler_flags "-fsanitize=address ${compiler_flags}")
endif()

set(BUILD_FAKE_JNI_DEBUG OFF CACHE BOOL "")
set(BUILD_FAKE_JNI_TESTS OFF CACHE BOOL "")
set(BUILD_FAKE_JNI_EXAMPLES OFF CACHE BOOL "")

set(CMAKE_CXX_FLAGS ${compiler_flags})
set(CMAKE_C_FLAGS ${compiler_flags})

add_subdirectory(fake-jni)

file(GLOB_RECURSE BARON_SRC "src/**.cpp")
add_library(baron SHARED ${BARON_SRC})
target_link_libraries(baron fake-jni)
target_include_directories(baron PUBLIC include)
target_compile_options(baron PUBLIC
    -Wall
    -Wextra
#    -Werror
    -pedantic
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-nested-anon-types
    -Wno-vla-extension
    -Wno-zero-length-array
)

set(BUILD_BARON_EXAMPLES ON CACHE BOOL "Enables baron example")

if(${BUILD_BARON_EXAMPLES})
    add_subdirectory(example)
endif()