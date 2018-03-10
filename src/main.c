#include "lowlevel.h"
#include "dma__int.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "scheduler.h"
#include "ssd1322_oled.h"
#include <stdio.h>

int main(void)
{
	volatile int i;
	volatile uint32_t thetime;
	volatile int s;
	uint8_t test[31][127];
	volatile uint16_t tester;
	uint16_t gob = 0x14D;

	Low_init();
	Dma_init();
	
	Uart_init();
	Spi_init();
	
	Sched_init();
	Systick_init();

	Oled_init();

	while (1) {
		for ( tester = 511; tester > -1; tester-- )
		{
			Uart_send( " C ", 3 );
			Systick_delayTicks(10000);
			//Spi_send ( &tester, 1 );
			Systick_delayTicks(1000);
		}
	}
	return 0;
}