#pragma once
#include <stdbool.h>

enum ERROR
{
	MISSING_ARGUMENT = 0,
	CONVERSATION_ERROR = 1,
	WRONG_SEQUENCE = 2,
};

void log_error(enum ERROR error, const char *option);

// function terminates program
int __attribute__((__noreturn__)) usage(int opt);