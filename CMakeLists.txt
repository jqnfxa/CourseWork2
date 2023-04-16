cmake_minimum_required(VERSION 3.24)
project(CourseWork C)

set(CMAKE_C_STANDARD 17)
add_executable(CourseWork main.c BmpLib/bmp_24.h Geometry/matrix.h Geometry/structures.h Geometry/matrix.c Draw/image.h Draw/image.c Validator/validator.h Validator/validator.c BmpLib/bmp_parser.h BmpLib/bmp_parser.c CLI/command_parser.h CLI/query_structures.h CLI/command_parser.c ExceptionHandler/logger.h ExceptionHandler/logger.c CLI/rectangle_parser.c CLI/types_parser.h CLI/types_parser.c CLI/circle_parser.c CLI/frame_parser.c CLI/rotate_parser.c Draw/rotate.h Draw/rotate.c Draw/frame.h Draw/frame.c Draw/Shapes/circle.c Draw/Shapes/circle.h Draw/Shapes/rectangle.c Draw/Shapes/rectangle.h Draw/Shapes/polygon.h Draw/Shapes/polygon.c CLI/line_parser.c CLI/polygon_parser.c)

target_link_libraries(CourseWork m)