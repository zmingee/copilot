#include <avr/io.h>

void spi_init(void) {
    DDRB |= ((1<<DDB5)|(1<<DDB3)|(1<<DDB2)|(1<<DDB1));

    SPCR |= ((1<<SPE)|(1<<MSTR)|(1<<SPR0));
}

unsigned char spi_transmit(unsigned char data) {
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));

    return SPDR;
}
