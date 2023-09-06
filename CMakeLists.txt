cmake_minimum_required(VERSION 3.25)
project(2023_2_RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(2023_2_RayTracing main.cpp Camara.cpp Objeto.cpp Rayo.cpp vec3.cpp
        Luz.cpp
        Luz.h)
