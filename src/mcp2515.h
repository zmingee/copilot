/**
 * @file mcp2515.h
 * @brief Function prototypes for interfacing with the MCP2515 module
 *
 * This contains the prototypes and API for interfacing with the MCP2515
 * module, as well as any macros, constants, and global variables you may
 * require.
 *
 * @author Zane Mingee (breakingmatter)
 * @bug UNDER ACTIVE DEVELOPMENT
 */

/** @addtogroup mcp2515
 * @{
 */

#ifndef _MCP2515_H
#define _MCP2515_H

/**
 * @brief Initialize the MCP2515
 * @author Zane Mingee
 * @date 2017-06-15
 * @param port          AVR port to use for SPI slave/chip select
 * @param can_speed     Speed to set the CAN bus to
 * @return Message indicating success or failure
 */
unsigned char mcp2515_init(unsigned int port, unsigned char can_speed);

/**
 * @brief Read the current status of the MCP2515
 * @author Zane Mingee
 * @date 2017-06-15
 * @return SPI response from MCP2515
 */
unsigned char mcp2515_read_status(void);

/**
 * @brief Check if there are any CAN messages to receive.
 *        Reads the status of the MCP2515 module and interprets if a CAN
 *        message is available.
 * @author Zane Mingee
 * @date 2017-06-15
 * @return Message indicating if a message is available
 */
unsigned char mcp2515_check_receive(void);

#endif /* _MCP2515_H */

/** @}*/
