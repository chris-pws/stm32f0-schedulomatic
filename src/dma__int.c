#include "dma__int.h"

// ******* Dma_init *******
// Meta function that prepares the configured peripherals for DMA access.
//  Inputs: none
// Outputs: none
void Dma_init(void) 
{
	
	dma_uartTxInit();
	dma_spiTxInit();
	nvic_init();

}

// ******* nvic_init *******
// Initializes the Nested Vector Interrupt Controller.
//  Inputs: none
// Outputs: none
void nvic_init(void) 
{
	nvic_set_priority( NVIC_DMA1_CHANNEL4_5_IRQ, 0 );
	nvic_enable_irq( NVIC_DMA1_CHANNEL4_5_IRQ );
	nvic_set_priority( NVIC_DMA1_CHANNEL2_3_IRQ, 0 );
	nvic_enable_irq( NVIC_DMA1_CHANNEL2_3_IRQ );
	nvic_set_priority( NVIC_SPI1_IRQ, 0 );
	nvic_enable_irq( NVIC_SPI1_IRQ );
}

// ******* dma_uartTxInit *******
// Sets up DMA transfers between memory and the UART peripheral for 
// transmission.
//  Inputs: none
// Outputs: none
void dma_uartTxInit(void) 
{
	dma_channel_reset( DMA1, DMA_CHANNEL4 );
	dma_set_peripheral_address( DMA1, DMA_CHANNEL4, (uint32_t) &USART2_TDR );
	dma_set_read_from_memory( DMA1, DMA_CHANNEL4 );
	dma_enable_memory_increment_mode( DMA1, DMA_CHANNEL4 );
	dma_set_peripheral_size( DMA1, DMA_CHANNEL4, DMA_CCR_PSIZE_8BIT );
	dma_set_memory_size( DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT );
	dma_set_priority( DMA1, DMA_CHANNEL4, DMA_CCR_PL_HIGH );
}

// ******* dma1_channel4_5_isr *******
// Predefined DMA channel ISR function. Clears interrupt flags and signals
// that transfer has completed and channel is free.
//  Inputs: none
// Outputs: none
void dma1_channel4_5_isr(void) 
{

	uint32_t isr = DMA1_ISR;

	// Channel 4 is UART2_TX
	if ( ( isr & DMA_ISR_TCIF4 ) || ( isr & DMA_ISR_TEIF4 ) )
	{
		//gpio_toggle( GPIOB, GPIO3 ); 
		DMA1_IFCR |= DMA_IFCR_CGIF4;	//Clear flag
		//dma_channel_reset(DMA1, DMA_CHANNEL2);
		Sched_flagSignal( &Flag_DMA_Chan4 );
		
	}

}

// ******* dma_spiTxInit *******
// Sets up DMA transfers between memory and the SPI peripheral for 
// transmission.
//  Inputs: none
// Outputs: none
void dma_spiTxInit(void) {
	dma_channel_reset( DMA1, DMA_CHANNEL3 );
	dma_set_peripheral_address( DMA1, DMA_CHANNEL3, (uint32_t) &SPI1_DR );
	dma_set_read_from_memory( DMA1, DMA_CHANNEL3 );
	dma_enable_memory_increment_mode( DMA1, DMA_CHANNEL3 );
	dma_enable_peripheral_increment_mode(DMA1, DMA_CHANNEL3);
	//copying 9 bit words
	dma_set_peripheral_size( DMA1, DMA_CHANNEL3, DMA_CCR_PSIZE_16BIT );
	dma_set_memory_size( DMA1, DMA_CHANNEL3, DMA_CCR_MSIZE_16BIT );
	dma_set_priority( DMA1, DMA_CHANNEL3, DMA_CCR_PL_HIGH );
}

// ******* dma1_channel2_3_isr *******
// Predefined DMA channel ISR function. Clears interrupt flags and signals
// that transfer has completed and channel is free.
//  Inputs: none
// Outputs: none
void dma1_channel2_3_isr(void)
{

	uint32_t isr = DMA1_ISR;
	uint8_t s;
	char test[20];

	// Channel 3 is SPI1_TX
	if ( isr & DMA_ISR_TCIF3 )
	{
		//s = sprintf(test, "%lu", isr);
		//Uart_send(test, s);		
		DMA1_IFCR |= DMA_IFCR_CGIF3;	/* Clear flags */

		dma_disable_channel( DMA1, DMA_CHANNEL3 );
		
		/* Set SPI transmission interrupt (TXE) */
		spi_enable_tx_buffer_empty_interrupt(SPI1);
		//Uart_send(" - dma. ", 8);

		//gpio_toggle(GPIOB, GPIO3); 	/* LED2 on/off */

	}
	else
	{
		Uart_send(" print ", 7);
	}

}

void spi1_isr(void)
{
	uint16_t isr = SPI1_SR;
	uint8_t s;
	char test[20];

	if ( SPI_SR(SPI1) & SPI_SR_TXE )
	{
		//s = sprintf(test, "%lu", SPI_SR(SPI1));
		//Uart_send(test, s);
		//Uart_send(" - spi. ", 8);
		spi_disable_tx_buffer_empty_interrupt(SPI1);	
		spi_disable(SPI1);
		spi_disable_tx_dma(SPI1);
		Sched_flagSignal( &Flag_DMA_Chan3 );
	}

	else
	{
		Uart_send(" xnt ", 5);
	}
}

// ******* sys_tick_handler *******
// Predefined SysTick ISR function. Runs the event manager and increments the counter.
//  Inputs: none
// Outputs: none
void sys_tick_handler(void)
{
	Sched_runEventManager();

	// Roll over to 0 at UINT32_MAX
	if ( systickCount < UINT32_MAX )
	{
		systickCount++;
	}
	else
	{
		systickCount = 0;
	}

}