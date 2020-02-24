#ifndef CCIPHER_LOGGING_H
#define CCIPHER_LOGGING_H

#include <stdbool.h>
#include <stdarg.h>

// Represents the verbosity of logging
enum
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
};

// Set the log level
void set_level(int new_level);

// Log a message to stdout/stderr
bool cc_log(int log_level, const char *fmt, ...);

// Macros for logging
#define log_debug(...) cc_log(LOG_DEBUG, __VA_ARGS__)
#define log_info(...) cc_log(LOG_INFO, __VA_ARGS__)
#define log_warn(...) cc_log(LOG_WARN, __VA_ARGS__)
#define log_error(...) cc_log(LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) cc_log(LOG_FATAL, __VA_ARGS__)

#endif //CCIPHER_LOGGING_H
