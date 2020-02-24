#include "libccipher/logging.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

static const char *LEVELS[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
static const char *COLORS[] = {"[0;34m", "[0;32m", "[0;33m", "[0;31m", "[1;31m"};
static const char *RESET = "[0m";

static int LEVEL = LOG_INFO;

void set_level(int new_level)
{
	if (new_level > LOG_FATAL || new_level < LOG_DEBUG) {
		return;
	}

	LEVEL = new_level;
}

bool cc_log(int log_level, const char *fmt, ...)
{
	if (log_level < LEVEL) {
		return false;
	}

	// Print current time
	time_t t = time(NULL);
	struct tm *local = localtime(&t);

	char buf[16];
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", local)] = '\0';

	fprintf(stderr, "%s \033%s%s \033%s", buf, COLORS[log_level], LEVELS[log_level], RESET);

	// Print the log message
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
	fflush(stderr);

	return true;
}
