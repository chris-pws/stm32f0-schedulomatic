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
#include <stddef.h>

int main(void)
{
	
	size_t s;
	char test[64];
	volatile int i;
	int x,y,prev_x,prev_y = 0;
	/*
	volatile uint8_t frame_buffer[16];
	frame_buffer_t fb_t;
	volatile int32_t fb_flag;
	frame_bufferInit( &fb_t, 8, 16, 4, (uint8_t *) &frame_buffer, &fb_flag );
	*/
	Low_init();
	Dma_init();

	//Test_init( &a_test_table );
	
	Uart_init();
	Spi_init();
	
	Sched_init();
	Systick_init();
	
	
	//s = sprintf( test, " %lu ", RCC_CFGR );
	//Uart_send( test, s );

	while (1) {

		//Test_start( &a_test_table );

		Uart_send( "\n", 1 );
		
		/*
		for ( i = 0; i < 16; i++ )
		{
			s = sprintf( test, " %x ", frame_buffer[i] );
			Uart_send( test, s );
			Uart_send( " ", 1 );
		}
		Uart_send( "\n\n", 2 );

		prev_x = x - 1;
		if ( prev_x < 0 )
		{
			prev_x = 7;
			prev_y = y - 1;
			if ( prev_y < 0 ) prev_y = 3;
		}
		

		// set current pixel to F
		//buffer_pixelSet( &fb_t,  )
		// set last pixel to 0
		//buffer_pixelSet( &fb_t,  )
		// increment column
		x += 1;
		if ( x == 8 ) // reached end of column, increment line and reset column
		{
			x = 0;
			y += 1;
		}
		if ( y = 4 ) // reached last line, return to first line
		{
			y = 0;
		}
		*/
		Systick_delayTicks(50000);
		
	}
	return 0;
}