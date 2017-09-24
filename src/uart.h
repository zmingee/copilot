#ifndef UART_H
#define UART_H

void uart_init(void);

void uart_write(char *s);

int uart_write_c(char c, FILE *stream);

char uart_read(void);

#endif
