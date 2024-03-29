cmake_minimum_required(VERSION 3.18)

project(hpx_matrix_multiplication CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(HPX REQUIRED)

add_executable(multiply main.cpp matrix.h)
target_link_libraries(multiply HPX::hpx HPX::wrap_main HPX::iostreams_component)