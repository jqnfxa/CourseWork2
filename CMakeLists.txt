cmake_minimum_required(VERSION 3.24)
project(CourseWork C)

set(CMAKE_C_STANDARD 17)

add_executable(CourseWork main.c CLI/cli.c CLI/cli.h ExceptionHandler/error_handler.c ExceptionHandler/error_handler.h ExceptionHandler/hint.h CLI/rectangle_parser.c CLI/rectangle_parser.h Shared/structures.h CLI/types_parser.c CLI/types_parser.h CLI/circle_parser.c CLI/circle_parser.h CLI/frame_parser.c CLI/frame_parser.h CLI/rotate_parser.c CLI/rotate_parser.h BmpLib/bmp_parser.c BmpLib/bmp_parser.h Tests/test_cli.c Tests/test_cli.h Tests/test_BmpLib.c Tests/test_BmpLib.h)
