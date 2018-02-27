#include "lowlevel.h"
#include "dma__int.h"
#include "uart.h"
#include "systick.h"
#include "scheduler.h"
#include <stdio.h>

int main(void)
{
	volatile int i;
	volatile uint32_t thetime;
	volatile int s;
	//uint8_t test[63][255];

	rcc_init();
	gpio_init();
	Uart_init();
	dma_uartTxInit();
	nvic_init();
	Sched_init();
	Systick_init();


	while (1) {

		for ( i = 0; i < 10000000; i++ );
		Uart_send( "Hello World!Hello World!Hello World!Hello World!\n\r", 50 );
	}
	return 0;
}