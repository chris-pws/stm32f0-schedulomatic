#include "uart.h"

// ******* Uart_init *******
// Initializes the USART peripheral for serial communication.
//  Inputs: none
// Outputs: none
void Uart_init(void) 
{

	usart_set_baudrate( USART2, 38400 );
	usart_set_databits( USART2, 8 );
	usart_set_stopbits( USART2, USART_CR2_STOP_1_0BIT );
	usart_set_mode( USART2, USART_MODE_TX );
	usart_set_parity( USART2, USART_PARITY_NONE );
	usart_set_flow_control( USART2, USART_FLOWCONTROL_NONE );
	usart_enable(USART2);

}

// ******* Uart_fifoTxEvent *******
// Periodic event that manages the UART transmit queue.
// Executed from the event scheduler.
//  Inputs: buffer_fifo_t pointer, signal flag
// Outputs: none
void Uart_fifoTxEvent( buffer_param_t *buffer, int32_t *flagPt )
{
	uint16_t buf[1], len;
	
	// attempt to read a byte from the buffer.
	if ( ( len = Buffer_get( &buf, buffer, 1 ) ) )
	{
		// Wait until transfer is complete
		Sched_flagWait(flagPt);
		// Copy byte to the buffer handler function
		buffer->is.fifo_u8->handler_function( &buf, len );

	}

	//gpio_toggle(GPIOB, GPIO3);
}

// ******* Uart_dmaTxHandler *******
// Copies a series of data from a memory address to the serial peripheral DMA
// transmission channel.
//  Inputs: pointer to a contiguous block of data, the number of bytes.
// (Depending on MSIZE and PSIZE settings in dma__int).
// Outputs: none
void Uart_dmaTxHandler( volatile void* data, uint8_t length ) 
{

	dma_disable_channel( DMA1, DMA_CHANNEL4 );

	dma_set_memory_address( DMA1, DMA_CHANNEL4, (uint32_t) data );
	dma_set_number_of_data( DMA1, DMA_CHANNEL4, length );

	dma_enable_transfer_complete_interrupt( DMA1, DMA_CHANNEL4 );
	dma_enable_channel( DMA1, DMA_CHANNEL4 );
	usart_enable_tx_dma(USART2);

}

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
void Uart_send( volatile void* data, uint32_t length ) 
{

	Buffer_put( data, &fifo_uartTx_param, length );

}
