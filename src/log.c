#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"

extern unsigned long timer1_millis;
extern unsigned long millis();
int LOG_LEVEL = DEBUG;

static const char *LEVEL_STRING[] = {
    "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG"
};

void log_print(char* filename, const char* funcname, int level, int line,
        char *fmt,...) {
    va_list list;
    char *p, *r;
    int e;

    if (level > LOG_LEVEL)
        return;


    fprintf(stdout, "[%-8s]", LEVEL_STRING[level]);
    fprintf(stdout, "[%s:%s:%d] ", filename, funcname, line);
    va_start(list, fmt);

    for(p = fmt; *p; ++p) {
        if (*p != '%' ) {
            fputc( *p, stdout );
        } else {
            switch ( *++p ) {
                /* string */
                case 's': {
                    r = va_arg(list, char *);
                    fprintf(stdout,"%s", r);
                    continue;
                }
                /* integer */
                case 'd': {
                    e = va_arg(list, int);
                    fprintf(stdout, "%d", e);
                    continue;
                }
                default: {
                    fputc(*p, stdout);
                }
            }
        }
    }
    va_end(list);
    fputc('\r', stdout);
    fputc('\n', stdout);
}
