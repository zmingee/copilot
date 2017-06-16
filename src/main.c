#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "spi.h"
#include "mcp2515.h"
#include "mcp_can_dfs.h"


int main(void) {
    unsigned char res;

    /* Set up the serial port */
    uart_init();

    /* The Arduino is now booted up and ready. */
    uart_write("Boot OK\r\n");

    /* Set up SPI */
    spi_init();
    uart_write("SPI initialized\r\n");

    /* Set up mcp2515 */
    while (1) {
        res = mcp2515_init(PB1, CAN_500KBPS);
        if (res == CAN_OK) {
            uart_write("MCP initialized\r\n");
            break;
        } else {
            uart_write("Failed to initialize MCP\r\n");
            uart_write("Resetting\r\n");
        }
    }

    return 0;
}
