#pragma once

extern const char *hint;
extern const char *error_messages[];

#define debug

enum ERROR_CODE {
	FILE_OPEN = 0,
	FILE_READ,
	FILE_WRITE,
	FILE_INVALID,
	UNSUPPORTED_TYPE,
	OUT_OF_MEMORY,
	WRONG_ARGUMENT_NUMBER,
	INPUT_INVALID,
	MISSING_ARGUMENT,
	CONVERSATION,
	AMBIGUOUS_CALL,
	INVALID_QUERY,
};

void log_error(enum ERROR_CODE code, const char *extra_data);

// function terminates program
int __attribute__((__noreturn__)) print_usage(int opt);