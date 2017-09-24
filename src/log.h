#ifndef LOG_H
#define LOG_H

enum Level {
    CRITICAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

extern int LOG_LEVEL;

void log_print(char* filename, const char* funcname, int level, int line,
    char *fmt,...);
#define LOG_PRINT(level, ...) log_print(__FILE__, __func__, level, __LINE__, __VA_ARGS__ )

#endif
