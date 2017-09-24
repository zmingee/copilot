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

int lenHelper(unsigned x) {
    if(x>=1000000000) return 10;
    if(x>=100000000) return 9;
    if(x>=10000000) return 8;
    if(x>=1000000) return 7;
    if(x>=100000) return 6;
    if(x>=10000) return 5;
    if(x>=1000) return 4;
    if(x>=100) return 3;
    if(x>=10) return 2;
    return 1;
}

int printLen(unsigned long x) {
    return x<0 ? lenHelper(-x)+1 : lenHelper(x);
}

char *print_time() {
    int size = 0;
    unsigned long t;
    char *buf;

    /* Get current time and convert to ISO 8601 */
    t = millis();

    /* Add +2 to size for square braces */
    size = printLen(t) + 1 + 2;
    buf = (char*)malloc(size);

    memset(buf, 0x0, size);
    snprintf(buf, size, "[%lu]", t);

    return buf;
}

void log_print(char* filename, const char* funcname, int level, int line,
        char *fmt,...) {
    va_list list;
    char *p, *r;
    int e;
    char *formatted_time;

    if (level > LOG_LEVEL)
        return;

    formatted_time = print_time();

    fprintf(stdout, "%s", formatted_time);
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

    free(formatted_time);
}
