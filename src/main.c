#include "test.h"

#include "lowlevel.h"
#include "dma__int.h"
#include "uart.h"
#include "spi.h"
#include "systick.h"
#include "scheduler.h"

#include "ssd1322_oled.h"
#include "frame.h"

#include <libopencm3/stm32/f0/rcc.h>
#include <stdio.h>

int main(void)
{
	
	volatile uint16_t tester;
	/*
	volatile uint32_t thetime;
	uint8_t s;
	char test[64];
	volatile int i;
	uint16_t gob = 0x14D;
	*/

	volatile uint8_t frame_buffer[16];
	frame_buffer_t fb;
	volatile int32_t fb_flag;
	frame_bufferInit( &fb, 4, 16, (uint8_t *) &frame_buffer, &fb_flag );

	Low_init();
	Dma_init();

	Test_init( &a_test_table );
	

	Uart_init();
	Spi_init();
	
	Sched_init();
	Systick_init();

	//Oled_init();

	
	//Test_init( &a_test_table );
	
	//Oled_test();	
	
	
	//s = sprintf( test, " %lu ", RCC_CFGR );
	//Uart_send( test, s );

	while (1) {
		Test_start( &a_test_table );
		for ( tester = 0; tester < 5; tester++ )
		{
			Systick_delayTicks(10000);
			Uart_send( "\n", 1 );
			//gpio_toggle(GPIOB, GPIO8);
			//Systick_delayTicks(1);
			
		}
		//Uart_send( " meow ", 6 );
		Test_end( &a_test_table );
		Test_calculate( &a_test_table );
		Systick_delayTicks(50000);
		
	}
	return 0;
}