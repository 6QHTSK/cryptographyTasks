cmake_minimum_required(VERSION 3.20)
project(T6 C)

set(CMAKE_C_STANDARD 99)

add_executable(T6 main.c)

include_directories("/usr/include/x86_64-linux-gnu")
include_directories("/usr/lib/x86_64-linux-gnu")
target_link_libraries(T6 "/usr/lib/x86_64-linux-gnu/libgmp.a")
target_link_libraries(T6 "/usr/lib/x86_64-linux-gnu/libgmpxx.a")
target_link_libraries(T6 "/usr/lib/x86_64-linux-gnu/libssl.a")
