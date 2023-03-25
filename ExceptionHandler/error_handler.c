#include "error_handler.h"
#include "hint.h"
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

	fprintf(fd, "%s", hint);

	exit(error ? EXIT_FAILURE : EXIT_SUCCESS);
}