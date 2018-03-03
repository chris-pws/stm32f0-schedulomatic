#include "spi.h"

// ******* Spi_init *******
// Initializes the SPI peripheral for simplex serial transmission.
// 9 bits per word
//  Inputs: none
// Outputs: none
void Spi_init(void)
{
	spi_reset(SPI1);

	spi_i2s_mode_spi_mode(SPI1);

	spi_init_master( SPI1, SPI_CR1_BR_FPCLK_DIV_256, 
		SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, 
		SPI_CR1_CRCL_8BIT, SPI_CR1_MSBFIRST );

	spi_set_data_size( SPI1, SPI_CR2_DS_9BIT );

	spi_enable_ss_output(SPI1);
	spi_set_bidirectional_transmit_only_mode(SPI1);

	spi_set_next_tx_from_buffer(SPI1);

	//Spi_csnHigh();
	spi_enable(SPI1);



}

// ******* Spi_fifoTxEvent *******
// Periodic event that manages the SPI transmission queue.
// Executed from the event scheduler.
//  Inputs: buffer_param_t pointer, signal flag
// Outputs: none
void Spi_fifoTxEvent( buffer_param_t *buffer, int32_t *flagPt )
{
	uint16_t buf[1], len;

	// attempt to read an element from the buffer.
	if ( ( len = Buffer_get( &buf, buffer, 1 ) ) )
	{
		// Wait until transaction is complete
		Sched_flagWait(flagPt);
		//Spi_begin();
		//Spi_init();
		//dma_spiTxInit();
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
	dma_disable_channel( DMA1, DMA_CHANNEL3 );

	dma_set_memory_address( DMA1, DMA_CHANNEL3, (uint32_t) data );
	dma_set_number_of_data( DMA1, DMA_CHANNEL3, length );

	dma_enable_transfer_complete_interrupt( DMA1, DMA_CHANNEL3 );
	dma_enable_channel( DMA1, DMA_CHANNEL3 );
	
	spi_enable_tx_dma(SPI1);
	
}

// ******* Spi_send *******
// Adds arbitrary number of elements to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, number of elements to copy
// Outputs: none
void Spi_send( volatile void* data, uint32_t length )
{
	Buffer_put( data, &fifo_spiTx_param, length );

}