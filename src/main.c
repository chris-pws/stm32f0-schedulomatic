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
	char buf[14];
	char test[] = "boob";
	volatile int s;
	//uint8_t test[63][255];

	rcc_init();
	gpio_init();
	Uart_init();
	dma_uartTxInit();
	nvic_init();
	Sched_Init();
	Systick_init();


	while (1) {

		for ( i = 0; i < 1000000; i++ );
	}
	return 0;
}