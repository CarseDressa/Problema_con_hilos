cmake_minimum_required(VERSION 3.10)
project(planificador CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(planificador src/main.cpp)
target_link_libraries(planificador Threads::Threads)
