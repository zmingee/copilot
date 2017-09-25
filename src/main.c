#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>

#include "log.h"
#include "uart.h"
#include "spi.h"
#include "mcp2515.h"
#include "mcp_can_dfs.h"

FILE uart_str = FDEV_SETUP_STREAM(uart_write_c, NULL, _FDEV_SETUP_RW);

bool boot(){
    /* Setup the serial port */
    uart_init();
    uart_write("Booting copilot...\r\n");
    uart_write("UART configured\r\n");
    stdout = &uart_str;
    LOG_PRINT(DEBUG, "Setup UART as STDOUT");

    /* Set up SPI */
    spi_init();
    LOG_PRINT(DEBUG, "SPI initialized");

    LOG_PRINT(DEBUG, "Boot successful");

    return true;
}

int main(void) {
    unsigned char res;
    bool boot_status;

    boot_status = boot();
    if (!boot_status) {
        uart_write("Boot initialization failed!");
        return 0;
    }

    /* Set up mcp2515 */
    while (1) {
        res = mcp2515_init(PB1, CAN_500KBPS);
        if (res == CAN_OK) {
            LOG_PRINT(INFO, "MCP initialized");
            break;
        } else {
            LOG_PRINT(INFO, "Failed to initialize MCP");
            LOG_PRINT(INFO, "Resetting");
            _delay_ms(1000);
        }
    }

    struct CanMessage *msg = NULL;
    while (1) {
        if (mcp2515_check_receive() == CAN_MSGAVAIL) {
            LOG_PRINT(INFO, "CAN message receieved");
            mcp2515_read_msg(msg);
            if (msg->data == NULL) {
            } else {
                printf("---\r\n");
            }
            for (unsigned int i = 0; i < msg->data_len; i++) {
                printf("0x%02X", msg->data[i]);
            }
            free(msg);
            printf("\r\n");
            _delay_ms(100);
        } else {
            LOG_PRINT(WARNING, "No message available");
            _delay_ms(1000);
        }
    }

    return 0;
}
