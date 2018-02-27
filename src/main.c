#include "lowlevel.h"
#include "dma__int.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "scheduler.h"
#include <stdio.h>

int main(void)
{
	volatile int i;
	volatile uint32_t thetime;
	volatile int s;
	//uint8_t test[63][255];
	uint16_t tester = 0x13F;

	Low_init();
	Dma_init();
	
	Uart_init();
	Spi_init();
	
	Sched_init();
	Systick_init();


	while (1) {

		Uart_send( " cockle ", 8 );
		for ( i = 0; i < 1000000; i++ );
		Spi_send( &tester, 1 );
		for ( i = 0; i < 1000000; i++ );
		
	}
	return 0;
}