cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE JNI_SRC "jni/src/**.cpp")
#add_executable(jni ${JNI_SRC})
add_library(jni SHARED ${JNI_SRC})
target_include_directories(jni PUBLIC jni/include)

file(GLOB_RECURSE PROBE_SRC "probe/src/**.cpp")
add_executable(probe ${PROBE_SRC})
target_include_directories(probe PUBLIC probe/include)
target_link_libraries(probe baron)
add_dependencies(probe jni)