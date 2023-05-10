TARGET = CourseWork
COMPILER = gcc
C_FLAGS = -c -std=c17 -Wall
OBJECTS = main.o bmp_parser.o canvas_parser.o circle_parser.o command_parser.o frame_parser.o line_parser.o \
polygon_parser.o rectangle_parser.o rotate_parser.o types_parser.o ellipse.o polygon.o rectangle.o frame.o \
image.o rotate.o logger.o matrix.o validator.o

all: build clean

build: $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $(TARGET) -lm

main.o: BmpLib/bmp_parser.h CLI/command_parser.h ExceptionHandler/logger.h main.c
	$(COMPILER) $(C_FLAGS) main.c

# Bmp library

bmp_parser.o: BmpLib/bmp_parser.h BmpLib/bmp_structures.h BmpLib/bmp_parser.c
	$(COMPILER) $(C_FLAGS) BmpLib/bmp_parser.c

# Console list Interface

canvas_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/canvas_parser.c
	$(COMPILER) $(C_FLAGS) CLI/canvas_parser.c

circle_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.h CLI/types_parser.h CLI/circle_parser.c
	$(COMPILER) $(C_FLAGS) CLI/circle_parser.c

command_parser.o: CLI/command_parser.h Draw/Shapes/ellipse.h Draw/Shapes/polygon.h Draw/Shapes/rectangle.h \
			Draw/frame.h Draw/rotate.h ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.c
	$(COMPILER) $(C_FLAGS) CLI/command_parser.c

frame_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.h CLI/types_parser.h CLI/frame_parser.c
	$(COMPILER) $(C_FLAGS) CLI/frame_parser.c

line_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.h CLI/types_parser.h CLI/line_parser.c
	$(COMPILER) $(C_FLAGS) CLI/line_parser.c

polygon_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.h CLI/types_parser.h CLI/polygon_parser.c
	$(COMPILER) $(C_FLAGS) CLI/polygon_parser.c

rectangle_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.h CLI/types_parser.h CLI/rectangle_parser.c
	$(COMPILER) $(C_FLAGS) CLI/rectangle_parser.c

rotate_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/command_parser.h CLI/types_parser.h CLI/rotate_parser.c
	$(COMPILER) $(C_FLAGS) CLI/rotate_parser.c

types_parser.o: ExceptionHandler/logger.h Validator/validator.h CLI/types_parser.h CLI/types_parser.c
	$(COMPILER) $(C_FLAGS) CLI/types_parser.c

# Draw module

ellipse.o: Draw/Shapes/ellipse.h Validator/validator.h Draw/image.h Draw/Shapes/ellipse.c
	$(COMPILER) $(C_FLAGS) Draw/Shapes/ellipse.c

polygon.o: Draw/Shapes/polygon.h Validator/validator.h Draw/Shapes/polygon.c
	$(COMPILER) $(C_FLAGS) Draw/Shapes/polygon.c

rectangle.o: Draw/Shapes/rectangle.h Validator/validator.h Draw/image.h Draw/Shapes/rectangle.c
	$(COMPILER) $(C_FLAGS) Draw/Shapes/rectangle.c

frame.o: Geometry/matrix.h Validator/validator.h Draw/Shapes/ellipse.h Draw/Shapes/polygon.h Draw/image.h Draw/frame.h Draw/frame.c
	$(COMPILER) $(C_FLAGS) Draw/frame.c

image.o: ExceptionHandler/logger.h BmpLib/bmp_parser.h Draw/image.h Draw/image.c
	$(COMPILER) $(C_FLAGS) Draw/image.c

rotate.o: Validator/validator.h Draw/image.h Draw/rotate.h Draw/rotate.c
	$(COMPILER) $(C_FLAGS) Draw/rotate.c

# Logger

logger.o: ExceptionHandler/logger.h ExceptionHandler/logger.c
	$(COMPILER) $(C_FLAGS) ExceptionHandler/logger.c

# Geometry

matrix.o: Geometry/matrix.h ExceptionHandler/logger.h Validator/validator.h Geometry/matrix.c
	$(COMPILER) $(C_FLAGS) Geometry/matrix.c

# Validator

validator.o: ExceptionHandler/logger.h Validator/validator.h Validator/validator.c
	$(COMPILER) $(C_FLAGS) Validator/validator.c

clean:
	rm -rf *.o