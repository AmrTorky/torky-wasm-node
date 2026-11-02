cmake_minimum_required(VERSION 3.16)
project(torky-wasm-node CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -mavx2 -march=native -Wall -Wextra)
include_directories(include)

add_executable(test_wasm_node tests/test_wasm_node.cpp)
