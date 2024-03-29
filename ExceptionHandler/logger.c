#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const char *error_messages[] = {
	"Failed to open file",
	"Failed to read file",
	"Failed to write in file",
	"Failed to process file: file is not bmp or damaged",
	"Failed to process file: unsupported type",
	"The program has run out of memory",
	"Failed to validate query: wrong arguments number",
	"Failed to validate query: invalid input",
	"Failed to validate query: missing argument for",
	"Failed to convert",
	"Failed to validate query: ambiguous call",
	"Failed to validate query: wrong arguments"
};

void log_error(enum ERROR_CODE code, const char *extra_data)
{
#ifndef debug
	return;
#endif
	if(extra_data != NULL)
	{
		fprintf(stderr, "%s %s\n", error_messages[code], extra_data);
	}
	else
	{
		fprintf(stderr, "%s\n", error_messages[code]);
	}
}

// function terminates program
int __attribute__((__noreturn__)) print_usage(int32_t opt)
{
	int32_t error = (opt == '?');

	FILE *fd = error ? stderr : stdout;

	fprintf(fd, "%s", hint);

	exit(error ? EXIT_FAILURE : EXIT_SUCCESS);
}

const char *hint =
	"program usage: [options] <filename>.bmp\n"
	"\n"
	"options:\n"
	"\n"
	"\t-h, --help                             print this useful hint\n"
	"\t\n"
	"\t--info <filename>.bmp\t\t       prints info about bmp file\n"
 	"\t\n"
	"\t--rectangle [options] <filename>.bmp\n"
	"\t\n"
	"\t\t-s, --start x,y                set the upper left corner of rectangle\n"
	"\t\t\n"
	"\t\t-e, --end x,y                  set the right bottom corner of rectangle \n"
	"\t\t\n"
	"\t\t-w, --width pixels             (optional) the line width (by default width = 1) \n"
	"\t\t\n"
	"\t\t-c, --color color              color of lines in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --color \"#FF00FF\" (note: use \"\" to get correct input)\n"
	"\t\t\n"
	"\t\t-f, --fill color               (optional) fill color in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --fill \"#FF00FF\" (note: use \"\" to get correct input)\n\n"
	"\t\t-n, --new <new_filename>.bmp   (optional) if option called then new file will be created instead of rewriting \n"
	"\t\t\n"
	"\t\t\n"
	"\t--circle [options] <filename>.bmp \n"
	"\t\n"
	"\tWarning: circle can be specified only with rectangle or with point and radius \n"
	"\t[i.e. -s and -e or -p and -r can be used at time]\n"
	"\t Note: if specified rectangle is not square then ellipse will be drawn\n"
	"\t\n"
	"\t\t-s, --start x,y                upper left corner of the rectangle into which the circle will be inscribed\n"
	"\t\t\n"
	"\t\t-e, --end x,y                  the right bottom corner of rectangle into which the circle will be inscribed\n"
	"\t\t\n"
	"\t\t-p, --point x,y                coordinates of center dot\n"
	"\t\t\n"
	"\t\t-r, --radius r                 radius of circle\n"
	"\t\t\n"
	"\t\t-w, --width pixels             (optional) specifies the line width (by default width = 1) \n"
	"\t\t\n"
	"\t\t-c, --color color              color of lines in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --color \"#FF00FF\" (note: use \"\" to get correct input)\n"
	"\t\t\n"
	"\t\t-f, --fill color               (optional) fill color in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --fill \"#FF00FF\" (note: use \"\" to get correct input)\n\n"
	"\t\t-n, --new <new_filename>.bmp   (optional) if option called then new file will be created instead of rewriting \n"
	"\t\t\t\n"
	"\t\n"
	"\t--rotate [options] <filename>.bmp \n"
	"\t\n"
	"\t\t-d, --direction dir            (optional) direction to rotate can be 'left' or 'right' (default is left)\n"
	"\t\t\n"
	"\t\t-a, --angle angle              angle to rotate\n"
	"\t\t\n"
	"\t\t-s, --start x,y                set the upper left corner of area\n"
	"\t\t\n"
	"\t\t-e, --end x,y                  set the right bottom corner of area\n"
	"\t\t\n"
	"\t\t-n, --new <new_filename>.bmp   (optional) if option called then new file will be created instead of rewriting \n"
	"\t\t\n"
	"\t\t\n"
	"\t--frame [options] <filename>.bmp\n\n"
	"\t\t-t, --type type                specifies frame type (now supported 2 types)\n"
	"\t\t                               type 1: linear frame\n"
	"\t\t                               type 2: circle frame\n"
	"\t\t\n"
	"\t\t-c, --color color              color of lines in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --color \"#FF00FF\" (note: use \"\" to get correct input)\n"
	"\t\t\n"
	"\t\t-w, --width pixels             (optional) specifies the frame width (by default will auto calculate width)\n"
	"\t\t\n"
	"\t\t-n, --new <new_filename>.bmp   (optional) if option called then new file will be created instead of rewriting\n"
	"\t\t\n"
	"\t\t\n"
	"\t--line [options] <filename>.bmp\n\n"
	"\t\t-s, --start x,y                line begin\n"
	"\t\t\n"
	"\t\t-e, --end x,y                  line end\n"
	"\t\t\n"
	"\t\t-c, --color color              color of line in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --color \"#FF00FF\" (note: use \"\" to get correct input)\n"
	"\t\t\n"
	"\t\t-w, --width pixels             (optional) specifies the line width (by default width = 1)\n"
	"\t\t\n"
	"\t\t-n, --new <new_filename>.bmp   (optional) if option called then new file will be created instead of rewriting\n"
	"\t\t\n"
	"\t\t\n"
	"\t--polygon [options] <filename>.bmp\n\n"
	"\t\t-p, --points x1,y1,...,xn,yn   point to form polygon\n"
	"\t\t\n"
	"\t\t-w, --width pixels             (optional) specifies the lines width (by default width = 1)\n"
	"\t\t\n"
	"\t\t-c, --color color              color of lines in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --color \"#FF00FF\" (note: use \"\" to get correct input)\n"
	"\t\t\n"
	"\t\t-f, --fill color               (optional) fill color in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --fill \"#FF00FF\" (note: use \"\" to get correct input)\n\n"
	"\t\t-n, --new <new_filename>.bmp   (optional) if option called then new file will be created instead of rewriting\n"
	"\t\t\n"
	"\t\t\n"
	"\t--canvas [options]\n\n"
	"\t\t-w, --width pixels             width of image\n"
	"\t\t\n"
	"\t\t-h, --height pixels            height of image\n"
	"\t\t\n"
	"\t\t-c, --color color              color of background in hex format; color from [#000000; #FFFFFF]\n"
	"\t\t                               example: --color \"#FF00FF\" (note: use \"\" to get correct input)\n"
	"\t\t\n"
	"\t\t-n, --new <new_filename>.bmp   specifies name of output file (the existing file will be overwritten)\n";