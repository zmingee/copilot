#ifndef MCP2515_H
#define MCP2515_H

void mcp2515_reset(void);

unsigned char mcp2515_read_register(unsigned char address);

void mcp2515_modify_register(unsigned char address, unsigned char mask,
        unsigned char data);

unsigned char mcp2515_set_CANCTRL_Mode(unsigned char newmode);

unsigned char mcp2515_init(unsigned int port, unsigned char can_speed);

unsigned char mcp2515_check_receive(void);

#endif
