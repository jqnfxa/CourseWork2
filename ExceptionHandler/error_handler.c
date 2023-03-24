#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>

void log_error(enum ERROR error, const char *option)
{
	switch(error)
	{
		case MISSING_ARGUMENT:
			fprintf(stderr, "option '%s' missing argument\n", option);
			break;
		case CONVERSATION_ERROR:
			fprintf(stderr, "invalid argument for option '%s'\n", option);
			break;
		case WRONG_SEQUENCE:
			fprintf(stderr, "option '%s' can't be called: wrong request type\n", option);
			break;
		default:
			break;
	}
}

int __attribute__ ((__noreturn__)) usage(int opt)
{
	int error = (opt == '?');

	FILE *fd = error ? stderr : stdout;

	fprintf(fd, "program usage: [options] <filename>.bmp\n\n");
	fprintf(fd, "-h, --help               display this text and exit\n");
	fprintf(fd, "--rectangle [-s, --start x,y] [-e, --end x,y] [-w, --width pixels] [-c, --color color] [-f, --fill fill_color] [-n, --new <new_filename>.bmp] <filename>.bmp\n");
	fprintf(fd, "--circle [-s, --start x,y] [-e, --end x,y] [-w, --width pixels] [-c, --color color] [-f, --fill fill_color] [-n, --new <new_filename>.bmp] <filename>.bmp\n");
	fprintf(fd, "--circle [-p, --point x,y] [-r, --radius x] [-w, --width pixels] [-c, --color color] [-f, --fill fill_color] [-n, --new <new_filename>.bmp] <filename>.bmp\n");
	fprintf(fd, "--rotate [-d, --direction left | right] [-a, --angle angle] [-s, --start x,y] [-e, --end x,y] [-n, --new <new_filename>.bmp] <filename>.bmp\n");
	fprintf(fd, "--frame [-t, --type integer] [-c, --color color] [-w, --width pixels] [-n, --new <new_filename>.bmp] <filename>.bmp\n");

	exit(error ? EXIT_FAILURE : EXIT_SUCCESS);
}