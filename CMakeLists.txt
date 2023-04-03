cmake_minimum_required(VERSION 3.24)
project(CourseWork C)

set(CMAKE_C_STANDARD 17)

add_executable(CourseWork main.c interfaces/BMPLib/bmp_parser.h interfaces/CLI/circle_parser.h interfaces/CLI/cli.h interfaces/CLI/frame_parser.h interfaces/CLI/rectangle_parser.h interfaces/CLI/rotate_parser.h interfaces/CLI/types_parser.h interfaces/Declarations/structures.h interfaces/ExceptionHandler/error_handler.h interfaces/ExceptionHandler/hint.h source/BmpLib/bmp_parser.c source/CLI/circle_parser.c source/CLI/cli.c source/CLI/frame_parser.c source/CLI/rectangle_parser.c source/CLI/rotate_parser.c source/CLI/types_parser.c source/ExceptionHandler/error_handler.c Tests/test_cli.c Tests/test_cli.h Tests/test_BmpLib.c Tests/test_BmpLib.h interfaces/Draw/rectangle.h source/Draw/rectangle.c interfaces/Draw/circle.h source/Draw/circle.c interfaces/Draw/rotate.h source/Draw/rotate.c source/ExceptionHandler/hint.c interfaces/Geometry/matrix.h source/Geometry/matrix.c)
