#ifndef RF_H_
#include "rf.h"
#endif

void rf_init(bool dir) {
	rf_power_down();
	spi_clear(CE);
	spi_cs_high();
	volatile int i;
	for (i=0; i<100000; i++);
	
	rf_write_reg_byte(NRF_REG_CONFIG, 0x08);
	rf_write_reg_byte(NRF_REG_SETUP_RETR, 0x5F);
	rf_clear_irq();
	//rf_power_up();
	for (i=0; i<100000; i++);

	//rf_write_reg_byte(NRF_REG_FEATURE, 0x06);
	//rf_write_reg_byte(NRF_REG_DYNDP, 0x3f);
	
	//MultiCeiver PTX must have identical tx & rx addr
	rf_set_rx_addr_p1(TX_ADDR);
	//rf_set_rx_addr_p0(TX_ADDR);
	rf_set_tx_addr(TX_ADDR);
	
	rf_write_reg_byte(NRF_REG_PW_P0, 0x20);
	rf_write_reg_byte(NRF_REG_PW_P1, 0x20);
	rf_clear_irq();
	//rf_write_reg_byte(NRF_REG_EN_RXADDR, 0x02);
	rf_flush_all();
	
	


	rf_write_reg_byte(NRF_REG_CONFIG, 0x0e);
	//rf_write_reg_byte(NRF_REG_EN_RXADDR, 0x03);
	rf_flush_all();
	for (i=0; i<10000; i++);
}

//Set receive pipe 0 address
void rf_set_rx_addr_p0(uint64_t address) {		
	char* data = (char*) &address;
	rf_write_reg(NRF_REG_RX_ADDR_P0);
	rf_spi_write_data(&data[0], 5);
	spi_end();

}

//Set receive pipe 1 address
void rf_set_rx_addr_p1(uint64_t address) {		
	char* data = (char*) &address;
	rf_write_reg(NRF_REG_RX_ADDR_P1);
	rf_spi_write_data(&data[0], 5);
	spi_end();

}

//Set receive pipe 2 address
void rf_set_rx_addr_p2(int address) {		
	rf_write_reg_byte(NRF_REG_RX_ADDR_P2, address);

}

//Set receive pipe 3 address
void rf_set_rx_addr_p3(int address) {		
	rf_write_reg_byte(NRF_REG_RX_ADDR_P3, address);

}

//Set receive pipe 4 address
void rf_set_rx_addr_p4(int address) {		
	rf_write_reg_byte(NRF_REG_RX_ADDR_P4, address);

}

//Set receive pipe 5 address
void rf_set_rx_addr_p5(int address) {		
	rf_write_reg_byte(NRF_REG_RX_ADDR_P5, address);

}

//Set transmission address
void rf_set_tx_addr(uint64_t address) {		
	char* data = (char*) &address;
	rf_write_reg(NRF_REG_TX_ADDR);
	rf_spi_write_data(&data[0], 5);
	spi_end();

}

//Write payload for transmission
void rf_write_tx_payload(void* data, int length) {		
	rf_spi_command(NRF_CMD_WRITE_TX_PAYLOAD);
	rf_spi_write_data(data, length);
	spi_end();

}

//Write payload for transmission without requesting acknowledge
void rf_write_tx_payload_no_ack(void* data, int length) {		
	rf_spi_command(NRF_CMD_WRITE_TX_PAYLOAD_NO_ACK);
	rf_spi_write_data(data, length);
	spi_end();

}

//Write payload for acknowledge
void rf_write_ack_payload(int pipe, void* data, int length) {		
	rf_spi_command(NRF_CMD_WRITE_ACK_PAYLOAD | (pipe & 0x7));
	rf_spi_write_data(data, length);
	spi_end();

}

//Read received payload
void rf_read_rx_payload(void* data, int length) {		
	rf_spi_command(NRF_CMD_READ_RX_PAYLOAD);
	rf_spi_read_data(data, length);
	spi_end();

}

//Set radio frequency in MHz (2400 to 2525 allowed)
void rf_set_frequency(int freq) {		
	rf_write_reg_byte(NRF_REG_RF_CH, (freq - 2400) & 0x7F);

}

//Get radio frequency in MHz
int rf_get_frequency(void) {		
	return (rf_read_reg_byte(NRF_REG_RF_CH) + 2400);
}

//Turn on radio
void rf_power_up() {
	//spi_set(CE);		
	int tmp = rf_read_reg_byte(NRF_REG_CONFIG);
	if ((tmp & (NRF_CONFIG_PWR_UP)) != (NRF_CONFIG_PWR_UP)) {
		tmp |= (NRF_CONFIG_PWR_UP);
		rf_write_reg_byte(NRF_REG_CONFIG, tmp);	//Update if we need
	}
}

//Turn off radio
void rf_power_down() {
	//spi_clear(CE);		
	int tmp = rf_read_reg_byte(NRF_REG_CONFIG);
	
	if (tmp & (NRF_CONFIG_PWR_UP)) {
		tmp &= ~(NRF_CONFIG_PWR_UP);
		rf_write_reg_byte(NRF_REG_CONFIG, tmp);	//Update if we need
	}
	
}

//Set up radio for transmission
void rf_mode_tx() {		
	int tmp = rf_read_reg_byte(NRF_REG_CONFIG);
	if (tmp & (NRF_CONFIG_PRIM_RX)) {
		tmp &= ~(NRF_CONFIG_PRIM_RX);
		rf_write_reg_byte(NRF_REG_CONFIG, tmp);	//Update if we need
	}
}

//Set up radio for reception
void rf_mode_rx() {		
	int tmp = rf_read_reg_byte(NRF_REG_CONFIG);
	if ((tmp & (NRF_CONFIG_PRIM_RX)) != (NRF_CONFIG_PRIM_RX)) {
		tmp |= (NRF_CONFIG_PRIM_RX);
		rf_write_reg_byte(NRF_REG_CONFIG, tmp);	//Update if we need
	}
}

//Enable autoack on pipe
void rf_enable_pipe_autoack(int pipes) {		
	int tmp = rf_read_reg_byte(NRF_REG_EN_AA);
	if ((tmp & (pipes)) != (pipes)) {
		tmp |= (pipes);
		rf_write_reg_byte(NRF_REG_EN_AA, tmp);	//Update if we need
	}
}

//Enable address on pipe
void rf_enable_pipe_address(int pipes) {		
	int tmp = rf_read_reg_byte(NRF_REG_EN_RXADDR);
	if ((tmp & (pipes)) != (pipes)) {
		tmp |= (pipes);
		rf_write_reg_byte(NRF_REG_EN_RXADDR, tmp);	//Update if we need
	}
}

//Enable dynamic payload length
void rf_enable_pipe_dlp(int pipes) {		
	int tmp = rf_read_reg_byte(NRF_REG_DYNDP);
	if ((tmp & (pipes)) != (pipes)) {
		tmp |= (pipes);
		rf_write_reg_byte(NRF_REG_DYNDP, tmp);	//Update if we need
	}
}

//Enabled various features
void rf_enable_features(int features) {		
	int tmp = rf_read_reg_byte(NRF_REG_FEATURE);
	if ((tmp & (features)) != (features)) {
		tmp |= (features);
		rf_write_reg_byte(NRF_REG_FEATURE, tmp);	//Update if we need
	}
}

//Set power level
void rf_set_pwr_lvl(int level) {
	int tmp = rf_read_reg_byte(NRF_REG_SETUP_RF);

	if (level == 0) {
		tmp &= ~(0x06);
	} else {
		tmp |= (level << 1);
	} 
	rf_write_reg_byte(NRF_REG_SETUP_RF, tmp);
}

//Set data rate
void rf_set_datarate(rf_datarate_e rate) {
	int tmp = rf_read_reg_byte(NRF_REG_SETUP_RF);

	if ( rate == NRF_DR_250KBPS ) {
		tmp = (tmp & ~(0x08)) | 0x20;
	} else {
		if ( rate == NRF_DR_1MBPS ) {
			tmp = (tmp & ~(0x20)) | 0x08;
		} else {
			// this is the power on default NRF_DR_2MBPS
			tmp &= ~(0x28);
		}
	}
	rf_write_reg_byte(NRF_REG_SETUP_RF, tmp);
}





void rf_spi_command(char command) {	
	spi_begin();
	spi_transfer(0, &command, 1);
}

void rf_write_reg_byte(int reg, char value) {
	rf_write_reg(reg);
	spi_transfer(0, &value, 1);
	spi_end();
}

void rf_write_reg(int reg) {
	rf_spi_command(NRF_CMD_WRITE_REGISTER | reg);	
}
void rf_read_reg(int reg) {
	rf_spi_command(NRF_CMD_READ_REGISTER | reg);	
}


char rf_read_reg_byte(int reg) {
	char result;
	rf_read_reg(reg);
	
	spi_transfer(&result, 0, 1);	
	spi_end();
	return result;
}

void rf_spi_write_data(char* buf, int length) {
	spi_transfer(0, buf, length);
}

void rf_spi_read_data(char* buf, int length) {
	spi_transfer(buf, 0, length);
}

void rf_spi_read_data_rev(char* buf, int length) {
	char* ptr = buf;
	volatile int i;
	for (i=0; i!=length;i++) {
		spi_transfer(ptr--, 0, 1);
	}
}

void rf_spi_write_data_rev(char* buf, int length) {
	char* ptr = buf;
	volatile int i;
	for (i=0; i!=length;i++) {
		spi_transfer(0, ptr--, 1);
	}
}



void rf_flush_tx() {
	rf_spi_command(NRF_CMD_FLUSH_TX);
	spi_end();
}
void rf_flush_rx() {
	rf_spi_command(NRF_CMD_FLUSH_RX);
	spi_end();
}


void rf_flush_all() {
	rf_flush_rx();
	rf_flush_tx();
}

void rf_clear_irq() {
	rf_write_reg_byte(NRF_REG_STATUS, NRF_STATUS_IRQ);
}

void rf_clear_rx_irq() {
	rf_write_reg_byte(NRF_REG_STATUS, NRF_STATUS_RX_DR);
}

void rf_clear_tx_irq() {
	rf_write_reg_byte(NRF_REG_STATUS, NRF_STATUS_TX_DS);
}

void rf_clear_maxrt_irq() {
	rf_write_reg_byte(NRF_REG_STATUS, NRF_STATUS_MAX_RT);
}

int rf_status() {
	char w = NRF_CMD_NOP;
	spi_begin();
	spi_transfer(&w, &w, 1);
	spi_end();	
	return w;
}

int rf_get_payload_width() {
	char w;
	rf_spi_command(NRF_CMD_READ_RX_PAYLOAD_WIDTH);
	spi_transfer(&w, 0, 1);
	spi_end();
	return w;
}

int rf_fifo_status() {
	return rf_read_reg_byte(NRF_REG_FIFO);
}