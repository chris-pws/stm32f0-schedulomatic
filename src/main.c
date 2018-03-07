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
	volatile uint16_t tester;
	uint16_t gob = 0x14D;

	Low_init();
	Dma_init();
	
	Uart_init();
	Spi_init();
	
	Sched_init();
	Systick_init();


	while (1) {
		for ( tester = 511; tester > -1; tester-- )
		{
			Uart_send( " crunch ", 8 );
			Systick_delayTicks(10);
			Spi_send( &tester, 1 );
			Systick_delayTicks(10);

		}
	}
	return 0;
}