#pragma once
#include <stdbool.h>

enum ERROR {
	MISSING_ARGUMENT = 0,
	CONVERSATION_ERROR,
	WRONG_SEQUENCE,
	AMBIGUOUS_CALL,
	OUT_OF_MEMORY
};

void log_error(enum ERROR error, const char *option);

// function terminates program
int __attribute__((__noreturn__)) usage(int opt);