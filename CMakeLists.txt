cmake_minimum_required(VERSION 3.28)
project(kv_store)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE "src/*.*")

add_executable(kv_store stress_test.cpp ${SOURCE})
