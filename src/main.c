#include "lowlevel.h"
#include "dma__int.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "scheduler.h"
#include "ssd1322_oled.h"
#include <libopencm3/stm32/f0/rcc.h>
#include <stdio.h>

int main(void)
{
	volatile int i;
	volatile uint32_t thetime;
	uint8_t s;
	char test[64];
	volatile uint16_t tester;
	uint16_t gob = 0x14D;

	Low_init();
	Dma_init();
	
	Uart_init();
	Spi_init();
	
	Sched_init();
	Systick_init();

	Oled_init();
	Oled_test();
	//s = sprintf( test, " %lu ", RCC_CFGR );
	//Uart_send( test, s );
	while (1) {
		for ( tester = 511; tester > -1; tester-- )
		{
			Systick_delayTicks(7500);
			Uart_send( " C ", 3 );
			//Systick_delayTicks(10000);
			
		}
	}
	return 0;
}