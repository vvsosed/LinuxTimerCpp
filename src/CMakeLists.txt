cmake_minimum_required(VERSION 3.5)
project(LinuxTimerCpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(tmrwrap)
#add_subdirectory(storage)
#add_subdirectory(jssp)
add_subdirectory(main)

