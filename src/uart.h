#ifndef UART_H
#define UART_H

void uart_init(void);

void uart_write(char *s);

char uart_read(void);

#endif
