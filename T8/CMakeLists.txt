cmake_minimum_required(VERSION 3.20)
project(T8)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(T8 main.cpp)

set(DEP_LIBS OpenSSL::Crypto OpenSSL::SSL)

target_link_libraries(T8 ${DEP_LIBS})
