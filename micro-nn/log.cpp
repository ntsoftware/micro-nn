#include "pch.h"

void log(enum log_category category, const char* format, ...) {
	const char* category_str = NULL;
	switch (category) {
	case LOG_I: category_str = ANSI_COLOR_BRIGHT_GREEN"I" ANSI_COLOR_DEFAULT; break;
	case LOG_W: category_str = ANSI_COLOR_BRIGHT_YELLOW "W" ANSI_COLOR_DEFAULT; break;
	case LOG_E: category_str = ANSI_COLOR_BRIGHT_RED "E" ANSI_COLOR_DEFAULT; break;
	}

	printf("[%s] ", category_str);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	putchar('\n');
}