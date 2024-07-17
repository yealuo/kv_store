cmake_minimum_required(VERSION 3.28)
project(kv_store)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE "src/*.*")

add_executable(stress_test stress_test.cpp ${SOURCE})
add_executable(function_test function_test.cpp ${SOURCE})
