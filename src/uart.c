#include <avr/io.h>
#include <util/setbaud.h>


/*
 * uart_init brings up the serial port
 */
void uart_init(void) {
    /*
     * The UBRR register is a UART baud rate register. We're using
     * UART 0. It's a 16-bit register, and we need to set the high
     * and low bytes to the values automatically provided for us by
     * the util/setbaud.h header.
     */
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    /*
     * UART control status registers (or UCSR0 for UART 0) control
     * the UART's operation. There are three such registers, labeled
     * A, B, and C.
     */

    /*
     * In UCSR0A, we want to disable 2x transmission speed.
     */
    UCSR0A &= ~(1 << U2X0);

    /*
     * Enable the transmitter (transmit enable 0) and receiver
     * (receive enable 0).
     */
    UCSR0B = ((1 << TXEN0)|(1 << RXEN0));

    /*
     * Now, we set the framing to the most common format: 8 data bits,
     * one stop bit.
     */
    UCSR0C = ((1 << UCSZ01)|(1 << UCSZ00));
}


/*
 * uart_write writes the string to the serial port.
 */
void uart_write(char *s) {
    int i = 0;

    while (s[i] != 0) {
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = s[i];
        i++;
    }
}


/*
 * uart_read blocks until a character is available from the UART,
 * returning that character when it is received.
 */
char uart_read(void) {
    /*
     * Similar to the write_string function, UCSR0A's RXC0 bit will
     * be set when a byte is ready to be read from the serial port.
     */
    loop_until_bit_is_set(UCSR0A, RXC0);

    /*
     * The contents of the UDR0 register have the byte that was
     * read in.
     */
    return UDR0;
}
