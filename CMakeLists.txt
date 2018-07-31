cmake_minimum_required(VERSION 3.12)
project(MOTOR_MONITOR)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MOTOR_MONITOR
        main.cpp
        Motor_Monitor.cpp
        Motor_Monitor.hpp)
