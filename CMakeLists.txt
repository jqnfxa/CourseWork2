cmake_minimum_required(VERSION 3.24)
project(CourseWork C)

set(CMAKE_C_STANDARD 17)

add_executable(CourseWork main.c CLI/cli.c CLI/cli.h ExceptionHandler/error_handler.c ExceptionHandler/error_handler.h)
