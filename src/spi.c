#include "spi.h"

// ******* Spi_init *******
// Initializes the SPI peripheral for simplex serial transmission.
// 9 bits per word
//  Inputs: none
// Outputs: none
void Spi_init(void)
{
	spi_reset(SPI1);

	SPI1_I2SCFGR = 0;

	spi_init_master(SPI1, SPI_CR1_BR_FPCLK_DIV_256, 
		SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_2, 
		SPI_CR1_DFF_16BIT, SPI_CR1_MSBFIRST);

	spi_enable_software_slave_management(SPI1);
	spi_set_nss_high(SPI1);

	Spi_csnHigh();
	spi_enable(SPI1);



}

// ******* Spi_fifoTxEvent *******
// Periodic event that manages the SPI transmission queue.
// Executed from the event scheduler.
//  Inputs: buffer_param_t pointer, signal flag
// Outputs: none
void Spi_fifoTxEvent( buffer_param_t *buffer, int32_t *flagPt )
{
	uint16_t buf[2], len;
	
	// attempt to read an element from the buffer.
	if ( ( len = Buffer_get( &buf, buffer, 1 ) ) )
	{
		// Wait until transaction is complete
		Sched_flagWait(flagPt);
		Spi_begin();
		// Send data to the buffer handler function
		buffer->is.fifo_u16->handler_function( &buf, len );

	}

	//gpio_toggle(GPIOB, GPIO3);
}

// ******* Spi_dmaTxHandler *******
// Copies data from a memory address to the SPI peripheral DMA transmission 
// channel.
//  Inputs: pointer to a contiguous block of data, number of elements to copy
// Outputs: none
void Spi_dmaTxHandler( volatile void* data, uint8_t length )
{
	dma_disable_channel(DMA1, DMA_CHANNEL3);

	dma_set_memory_address(DMA1, DMA_CHANNEL3, (uint32_t) data);
	dma_set_number_of_data(DMA1, DMA_CHANNEL3, length);

	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL3);
	dma_enable_channel(DMA1, DMA_CHANNEL3);
	
	spi_enable_tx_dma(SPI1);
}

// ******* Spi_send *******
// Adds arbitrary number of elements to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, number of elements to copy
// Outputs: none
void Spi_send( volatile void* data, uint32_t length )
{
	Buffer_put(data, &fifo_spiTx_param, length);

}

// ******* Spi_csnHigh *******
// Sets the software controlled chip select line high.
//  Inputs: none
// Outputs: none
void Spi_csnHigh(void) {
	gpio_set(SPI_PORT, CSN);
}

// ******* Spi_csnLow *******
// Sets the software controlled chip select line low.
//  Inputs: none
// Outputs: none
void Spi_csnLow(void) {
	gpio_clear(SPI_PORT, CSN);	
}

// ******* Spi_begin *******
// Called at the beginning of an SPI word transfer.
//  Inputs: none
// Outputs: none
void Spi_begin(void) {
	Spi_csnLow();
}

// ******* Spi_end *******
// Called at the end of an SPI word transfer.
//  Inputs: none
// Outputs: none
void Spi_end(void) {
	spi_disable_rx_dma(SPI1);
	Spi_csnHigh();
}