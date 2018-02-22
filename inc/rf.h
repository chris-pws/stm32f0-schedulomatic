#ifndef RF_H_
#include <stdint.h>
#include "spi.h"

typedef enum { NRF_DR_1MBPS = 0, NRF_DR_2MBPS, NRF_DR_250KBPS } rf_datarate_e;

struct packet_header {
	int magic;
	int command;
	int data_length;
};

void rf_init(bool dir);

void rf_spi_command(char command);
void rf_write_reg_byte(int reg, char value);
void rf_write_reg(int reg);
void rf_read_reg(int reg);
char rf_read_reg_byte(int reg);
void rf_spi_write_data(char* buf, int length);
void rf_spi_read_data(char* buf, int length);
void rf_spi_read_data_rev(char* buf, int length);
void rf_spi_write_data_rev(char* buf, int length);


void rf_set_rx_addr_p0(uint64_t address);
void rf_set_rx_addr_p1(uint64_t address);
void rf_set_rx_addr_p2(int address);
void rf_set_rx_addr_p3(int address);
void rf_set_rx_addr_p4(int address);
void rf_set_rx_addr_p5(int address);
void rf_set_tx_addr(uint64_t address);
void rf_write_tx_payload(void* data, int length);
void rf_write_tx_payload_no_ack(void* data, int length);
void rf_write_ack_payload(int pipe, void* data, int length);
void rf_read_rx_payload(void* data, int length);
void rf_set_frequency(int freq);
int rf_get_frequency(void);
void rf_power_up(void);
void rf_power_down(void);
void rf_mode_tx(void);
void rf_mode_rx(void);
void rf_enable_pipe_autoack(int pipes);
void rf_enable_pipe_address(int pipes);
void rf_enable_pipe_dlp(int pipes);
void rf_enable_features(int features);
void rf_set_pwr_lvl(int level);
void rf_set_datarate(rf_datarate_e rate);

void rf_flush_tx(void);
void rf_flush_rx(void);
void rf_flush_all(void);

void rf_clear_irq(void);
void rf_clear_rx_irq(void);
void rf_clear_tx_irq(void);
void rf_clear_maxrt_irq(void);
int rf_status(void);
int rf_get_payload_width(void);
int rf_fifo_status(void);


//Hand written! Don't lose!

#define NRF_CMD_WRITE_REGISTER 0x20
#define NRF_CMD_READ_REGISTER 0x0


#define NRF_STATUS_MAX_RT (1<<4)
#define NRF_STATUS_TX_DS (1<<5)
#define NRF_STATUS_RX_DR (1<<6)
#define NRF_STATUS_IRQ  (NRF_STATUS_MAX_RT | NRF_STATUS_TX_DS | NRF_STATUS_RX_DR)

#define NRF_FIFO_RX_EMPTY (1<<0)
#define NRF_FIFO_RX_FULL (1<<1)
#define NRF_FIFO_TX_EMPTY (1<<4)
#define NRF_FIFO_TX_FULL (1<<5)

#define NRF_REG_STATUS 0x07


//Automatic stuff



#define NRF_CONFIG_PRIM_RX 0x01
#define NRF_CONFIG_PWR_UP 0x02

#define NRF_FEATURE_ACK_PAYLOAD 0x02
#define NRF_FEATURE_DPL 0x04

#define NRF_MASK_PIPE0 0x01
#define NRF_MASK_PIPE1 0x02
#define NRF_MASK_PIPE2 0x04
#define NRF_MASK_PIPE3 0x08
#define NRF_MASK_PIPE4 0x10
#define NRF_MASK_PIPE5 0x20


#define NRF_REG_CONFIG 0x00
#define NRF_REG_EN_AA 0x01
#define NRF_REG_EN_RXADDR 0x02
#define NRF_REG_SETUP_RETR 0x04
#define NRF_REG_SETUP_RF 0x06
#define NRF_REG_DYNDP 0x1c
#define NRF_REG_RX_ADDR_P5 0x0f
#define NRF_REG_RX_ADDR_P1 0x0b
#define NRF_REG_RX_ADDR_P3 0x0d
#define NRF_REG_TX_ADDR 0x10
#define NRF_REG_FEATURE 0x1d
#define NRF_REG_RX_ADDR_P0 0x0a
#define NRF_REG_RF_CH 0x05
#define NRF_REG_RX_ADDR_P2 0x0c
#define NRF_REG_RX_ADDR_P4 0x0e

#define NRF_REG_PW_P0 0x11
#define NRF_REG_PW_P1 0x12
#define NRF_REG_PW_P2 0x13
#define NRF_REG_PW_P3 0x14
#define NRF_REG_PW_P4 0x15
#define NRF_REG_PW_P5 0x16

#define NRF_REG_FIFO 0x17

#define NRF_CMD_WRITE_ACK_PAYLOAD 0xa8
#define NRF_CMD_WRITE_TX_PAYLOAD 0xa0
#define NRF_CMD_WRITE_TX_PAYLOAD_NO_ACK 0xb0
#define NRF_CMD_READ_RX_PAYLOAD_WIDTH 0x60
#define NRF_CMD_READ_RX_PAYLOAD 0x61

#define NRF_CMD_NOP 0xFF

#define NRF_CMD_FLUSH_RX 0xE2
#define NRF_CMD_FLUSH_TX 0xE1

#define NRF_DR_250KBPS 0x00
#define NRF_DR_1MBPS 0x01
#define NRF_DR_2MBPS 0x02

#define NRF_PA_MIN 0x00
#define NRF_PA_LOW 0x01
#define NRF_PA_HIGH 0x02
#define NRF_PA_MAX 0x03

#define RF_H_
#endif