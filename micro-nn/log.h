#pragma once

enum log_category {
    LOG_I = 0,
    LOG_W = 1,
    LOG_E = 2,
};

void log(enum log_category category, const char* format, ...);

#define log_i(format, ...) log(LOG_I, format, __VA_ARGS__)
#define log_w(format, ...) log(LOG_W, format, __VA_ARGS__)
#define log_e(format, ...) log(LOG_E, format, __VA_ARGS__)

#define ANSI_COLOR_DEFAULT			"\x1B[0m"

#define ANSI_COLOR_BLACK			"\x1B[30m"
#define ANSI_COLOR_RED				"\x1B[31m"
#define ANSI_COLOR_GREEN			"\x1B[32m"
#define ANSI_COLOR_YELLOW		    "\x1B[33m"
#define ANSI_COLOR_BLUE				"\x1B[34m"
#define ANSI_COLOR_MAGENTA			"\x1B[35m"
#define ANSI_COLOR_CYAN				"\x1B[36m"
#define ANSI_COLOR_WHITE			"\x1B[37m"

#define ANSI_COLOR_BRIGHT_BLACK		"\x1B[90m"
#define ANSI_COLOR_BRIGHT_RED		"\x1B[91m"
#define ANSI_COLOR_BRIGHT_GREEN		"\x1B[92m"
#define ANSI_COLOR_BRIGHT_YELLOW	"\x1B[93m"
#define ANSI_COLOR_BRIGHT_BLUE		"\x1B[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA	"\x1B[95m"
#define ANSI_COLOR_BRIGHT_CYAN		"\x1B[96m"
#define ANSI_COLOR_BRIGHT_WHITE		"\x1B[97m"