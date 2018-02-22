#include "dma__int.h"

// ******* dma_uartTxInit *******
// Sets up DMA transfers between memory and the serial peripheral for 
// transmission.
// Inputs: none
// Outputs: none
void dma_uartTxInit(void) {
	dma_channel_reset(DMA1, DMA_CHANNEL4);
	dma_set_peripheral_address(DMA1, DMA_CHANNEL4, (uint32_t) &USART2_TDR);
	dma_set_read_from_memory(DMA1, DMA_CHANNEL4);
	dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL4);
	dma_set_peripheral_size(DMA1, DMA_CHANNEL4, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(DMA1, DMA_CHANNEL4, DMA_CCR_PL_VERY_HIGH);
	/*
	dma_channel_reset(DMA1, DMA_CHANNEL5);
	dma_set_peripheral_address(DMA1, DMA_CHANNEL5, (uint32_t) &USART2_RDR);
	dma_set_read_from_peripheral(DMA1, DMA_CHANNEL5);
	dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL5);
	dma_set_peripheral_size(DMA1, DMA_CHANNEL5, DMA_CCR_PSIZE_8BIT);
	dma_set_memory_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_8BIT);
	dma_set_priority(DMA1, DMA_CHANNEL5, DMA_CCR_PL_VERY_HIGH);
	*/
}

// ******* nvic_init *******
// Initilizes the Nested Vector Interrupt Controller.
// Inputs: none
// Outputs: none
void nvic_init(void) {
	nvic_set_priority(NVIC_DMA1_CHANNEL4_5_IRQ, 2);
	nvic_enable_irq(NVIC_DMA1_CHANNEL4_5_IRQ);
}

// ******* dma1_channel4_5_isr *******
// Built-in DMA channel ISR function. Clears interrupt flags and signals
// that transfer has completed and channel is free.
// Inputs: none
// Outputs: none
void dma1_channel4_5_isr(void) 
{

	uint32_t isr = DMA1_ISR;


	if ( ( isr & DMA_ISR_TCIF4 ) || ( isr & DMA_ISR_TEIF4 ) )
	{
		gpio_toggle(GPIOB, GPIO3); /* LED2 on/off */
		DMA1_IFCR |= DMA_IFCR_CGIF4;	//Clear flag
		//dma_channel_reset(DMA1, DMA_CHANNEL2);
		Sched_flagSignal( &Flag_DMA_Chan4 );
		
	}

}

// ******* sys_tick_handler *******
// Built-in SysTick ISR function. Runs the event manager and increments the counter.
// Inputs: none
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