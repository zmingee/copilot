/**
 * @file mcp2515.c
 * @brief Source file for interfacing with the MCP2515 module
 *
 * This is the implementation for the MCP2515 module. Includes all required
 * logic for utilizing the module.
 *
 * @author Zane Mingee (breakingmatter)
 * @bug UNDER ACTIVE DEVELOPMENT
 */

/** @addtogroup mcp2515
 * @{
 */


#include <avr/io.h>

#include "spi.h"
#include "mcp_can_dfs.h"

static int SPI_CS;

/**
 * @brief Reset the MCP2515.
 *        This just writes the MCP_RESET macro define to the SPI port.
 * @author Zane Mingee
 * @date 2017-06-15
 */
static void mcp2515_reset(void) {
    PORTB &= ~(1 << SPI_CS);
    spi_transmit(MCP_RESET);
    PORTB |= (1 << SPI_CS);
}

/**
 * @brief Set a given register to a given value.
 *        Writes MCP_WRITE to SPI port, then writes the address and value.
 * @author Zane Mingee
 * @date 2017-06-15
 * @param address   Address to set
 * @param value     Value to set the register to
 */
static void mcp2515_set_register(const unsigned char address,
        const unsigned char value) {
    PORTB &= ~(1 << SPI_CS);
    spi_transmit(MCP_WRITE);
    spi_transmit(address);
    spi_transmit(value);
    PORTB |= (1 << SPI_CS);
}

/**
 * @brief Read the value of a register.
 *        Writes MCP_READ to the SPI port, writes the address, then writes 0x00
 *        and returns the response.
 * @author Zane Mingee
 * @date 2017-06-15
 * @param address   Address to read
 * @return SPI response from MCP2515
 */
static unsigned char mcp2515_read_register(unsigned char address) {
    unsigned char ret;

    PORTB &= ~(1 << SPI_CS);
    spi_transmit(MCP_READ);
    spi_transmit(address);
    ret = spi_transmit(0x00);
    PORTB |= (1 << SPI_CS);

    return ret;
}

/**
 * @brief Modify a register using the mask specified.
 *        Writes MCP_BITMOD to the SPI port, writes the address, the mask,
 *        and the data to submit.
 * @author Zane Mingee
 * @date 2017-06-15
 * @param address   Register address to modify
 * @param mask      Mask to use when manipulating the register value
 * @param data      Data to assign to the register
 */
static void mcp2515_modify_register(unsigned char address, unsigned char mask,
        unsigned char data) {
    PORTB &= ~(1 << SPI_CS);
    spi_transmit(MCP_BITMOD);
    spi_transmit(address);
    spi_transmit(mask);
    spi_transmit(data);
    PORTB |= (1 << SPI_CS);
}

/**
 * @brief This function will set the MCP2515 mode.
 *        Modifies the MCP_CANCTRL register, then reads the register and
 *        verifies it was successful.
 * @author Zane Mingee
 * @date 2017-06-15
 * @param newmode   Mode to set the MCP2515 to
 * @return Message indicating success or failure
 */
static unsigned char mcp2515_set_CANCTRL_Mode(unsigned char newmode) {
    unsigned char i;

    mcp2515_modify_register(MCP_CANCTRL, MODE_MASK, newmode);

    i = mcp2515_read_register(MCP_CANCTRL);
    i &= MODE_MASK;

    if (i == newmode) {
        return MCP2515_OK;
    }

    return MCP2515_FAIL;
}

/**
 * @brief Set the MCP2515 CAN speed.
 *        Modifies the MCP_CNF1, MCP_CNF2, and MCP_CNF3 registers to the
 *        corresponding values for the specified speeds, then returns a success
 *        or failure message.
 * @author Zane Mingee
 * @date 2017-06-15
 * @param can_speed     Speed to set the CAN bus to
 * @return Message indicating success or failure
 */
static unsigned char mcp2515_config_rate(const unsigned char can_speed) {
    unsigned char set, cfg1, cfg2, cfg3;
    set = 1;

    switch (can_speed) {
        case (CAN_5KBPS):
            cfg1 = MCP_16MHz_5kBPS_CFG1;
            cfg2 = MCP_16MHz_5kBPS_CFG2;
            cfg3 = MCP_16MHz_5kBPS_CFG3;
            break;

        case (CAN_10KBPS):
            cfg1 = MCP_16MHz_10kBPS_CFG1;
            cfg2 = MCP_16MHz_10kBPS_CFG2;
            cfg3 = MCP_16MHz_10kBPS_CFG3;
            break;

        case (CAN_20KBPS):
            cfg1 = MCP_16MHz_20kBPS_CFG1;
            cfg2 = MCP_16MHz_20kBPS_CFG2;
            cfg3 = MCP_16MHz_20kBPS_CFG3;
            break;

        case (CAN_25KBPS):
            cfg1 = MCP_16MHz_25kBPS_CFG1;
            cfg2 = MCP_16MHz_25kBPS_CFG2;
            cfg3 = MCP_16MHz_25kBPS_CFG3;
            break;

        case (CAN_31K25BPS):
            cfg1 = MCP_16MHz_31k25BPS_CFG1;
            cfg2 = MCP_16MHz_31k25BPS_CFG2;
            cfg3 = MCP_16MHz_31k25BPS_CFG3;
            break;

        case (CAN_33KBPS):
            cfg1 = MCP_16MHz_33kBPS_CFG1;
            cfg2 = MCP_16MHz_33kBPS_CFG2;
            cfg3 = MCP_16MHz_33kBPS_CFG3;
            break;

        case (CAN_40KBPS):
            cfg1 = MCP_16MHz_40kBPS_CFG1;
            cfg2 = MCP_16MHz_40kBPS_CFG2;
            cfg3 = MCP_16MHz_40kBPS_CFG3;
            break;

        case (CAN_50KBPS):
            cfg1 = MCP_16MHz_50kBPS_CFG1;
            cfg2 = MCP_16MHz_50kBPS_CFG2;
            cfg3 = MCP_16MHz_50kBPS_CFG3;
            break;

        case (CAN_80KBPS):
            cfg1 = MCP_16MHz_80kBPS_CFG1;
            cfg2 = MCP_16MHz_80kBPS_CFG2;
            cfg3 = MCP_16MHz_80kBPS_CFG3;
            break;

        case (CAN_83K3BPS):
            cfg1 = MCP_16MHz_83k3BPS_CFG1;
            cfg2 = MCP_16MHz_83k3BPS_CFG2;
            cfg3 = MCP_16MHz_83k3BPS_CFG3;
            break;

        case (CAN_95KBPS):
            cfg1 = MCP_16MHz_95kBPS_CFG1;
            cfg2 = MCP_16MHz_95kBPS_CFG2;
            cfg3 = MCP_16MHz_95kBPS_CFG3;
            break;

        case (CAN_100KBPS):
            cfg1 = MCP_16MHz_100kBPS_CFG1;
            cfg2 = MCP_16MHz_100kBPS_CFG2;
            cfg3 = MCP_16MHz_100kBPS_CFG3;
            break;

        case (CAN_125KBPS):
            cfg1 = MCP_16MHz_125kBPS_CFG1;
            cfg2 = MCP_16MHz_125kBPS_CFG2;
            cfg3 = MCP_16MHz_125kBPS_CFG3;
            break;

        case (CAN_200KBPS):
            cfg1 = MCP_16MHz_200kBPS_CFG1;
            cfg2 = MCP_16MHz_200kBPS_CFG2;
            cfg3 = MCP_16MHz_200kBPS_CFG3;
            break;

        case (CAN_250KBPS):
            cfg1 = MCP_16MHz_250kBPS_CFG1;
            cfg2 = MCP_16MHz_250kBPS_CFG2;
            cfg3 = MCP_16MHz_250kBPS_CFG3;
            break;

        case (CAN_500KBPS):
            cfg1 = MCP_16MHz_500kBPS_CFG1;
            cfg2 = MCP_16MHz_500kBPS_CFG2;
            cfg3 = MCP_16MHz_500kBPS_CFG3;
            break;

        case (CAN_666KBPS):
            cfg1 = MCP_16MHz_666kBPS_CFG1;
            cfg2 = MCP_16MHz_666kBPS_CFG2;
            cfg3 = MCP_16MHz_666kBPS_CFG3;
            break;

        case (CAN_1000KBPS):
            cfg1 = MCP_16MHz_1000kBPS_CFG1;
            cfg2 = MCP_16MHz_1000kBPS_CFG2;
            cfg3 = MCP_16MHz_1000kBPS_CFG3;
            break;

        default:
            set = 0;
            break;
    }

    if (set) {
        mcp2515_set_register(MCP_CNF1, cfg1);
        mcp2515_set_register(MCP_CNF2, cfg2);
        mcp2515_set_register(MCP_CNF3, cfg3);
        return MCP2515_OK;
    } else {
        return MCP2515_FAIL;
    }
}

/**
 * @brief Initialize the MCP2515 CAN buffers.
 *        Walks through 0-13 and sets the MCP_TXB0CTRL, MCP_TXB1CTRL, and
 *        MCP_TXB2CTRL registers to the corresponding integers. Then, sets the
 *        MCP_RXB0CTRL and MCP_RXB1CTRL registers to 0.
 * @author Zane Mingee
 * @date 2017-06-15
 */
static void mcp2515_init_CAN_buffers(void) {
    unsigned char i;
    unsigned char a1 = MCP_TXB0CTRL;
    unsigned char a2 = MCP_TXB1CTRL;
    unsigned char a3 = MCP_TXB2CTRL;

    for (i = 0; i < 14; i++) {
        mcp2515_set_register(a1, 0);
        mcp2515_set_register(a2, 0);
        mcp2515_set_register(a3, 0);
        a1++;
        a2++;
        a3++;
    }
    mcp2515_set_register(MCP_RXB0CTRL, 0);
    mcp2515_set_register(MCP_RXB1CTRL, 0);
}

/**
 * @brief Initialize the MCP2515
 *        This will set put the MCP2515 into config mode, set the CAN speed,
 *        initialize the CAN buffers, setup the interrupt mode, and then put
 *        the MCP2515 back into normal operating mode.
 * @author Zane Mingee
 * @date 2017-06-15
 * @param port          AVR port to use for SPI slave/chip select
 * @param can_speed     Speed to set the CAN bus to
 * @return Message indicating success or failure
 */
unsigned char mcp2515_init(unsigned int port, unsigned char can_speed) {
    unsigned char res;

    SPI_CS = port;

    /* Reset MCP2515 */
    mcp2515_reset();

    res = mcp2515_set_CANCTRL_Mode(MODE_CONFIG);
    if (res > 0) {
        return res;
    }

    res = mcp2515_config_rate(can_speed);
    if (res > 0) {
        return res;
    }

    if (res == MCP2515_OK) {
        /* Init CAN buffers */
        mcp2515_init_CAN_buffers();

        /* Setup interrupt mode */
        mcp2515_set_register(MCP_CANINTE, MCP_RX0IF | MCP_RX1IF);

        mcp2515_modify_register(
            MCP_RXB0CTRL,
            MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
            MCP_RXB_RX_STDEXT | MCP_RXB_BUKT_MASK
        );
        mcp2515_modify_register(
            MCP_RXB1CTRL,
            MCP_RXB_RX_MASK,
            MCP_RXB_RX_STDEXT
        );

        res = mcp2515_set_CANCTRL_Mode(MODE_NORMAL);
        if (res) {
            return res;
        }
    }

    return CAN_OK;
}

/**
 * @brief Read the current status of the MCP2515.
 *        Writes MCP_READ_STATUS to SPI and returns the response.
 * @author Zane Mingee
 * @date 2017-06-15
 * @return SPI response from MCP2515
 */
unsigned char mcp2515_read_status(void) {
    unsigned char i;

    PORTB &= ~(1 << SPI_CS);
    spi_transmit(MCP_READ_STATUS);
    i = spi_transmit(0x00);
    PORTB |= (1 << SPI_CS);

    return i;
}

/**
 * @brief Check if there are any CAN messages to receive.
 *        Reads the status of the MCP2515 module and interprets if a CAN
 *        message is available.
 * @author Zane Mingee
 * @date 2017-06-15
 * @return Message indicating if a message is available
 */
unsigned char mcp2515_check_receive(void) {
    unsigned char res;

    res = mcp2515_read_status();
    if (res & MCP_STAT_RXIF_MASK) {
        return CAN_MSGAVAIL;
    } else {
        return CAN_NOMSG;
    }
}

/** @}*/
