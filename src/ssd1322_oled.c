#include "ssd1322_oled.h"

/******** Oled_init *********
* Initializes the display controller.
*  Inputs: none
* Outputs: none
*/
void Oled_init(void)
{
	uint8_t s;
	char test[20];

	uint16_t init_seq[] = 
	{
		OLED_DISP_OFF,
		OLED_SET_VDD, 0x101,
		OLED_CONTRAST_CURRENT, 0x180,
		OLED_MASTER_CONTRAST, 0x10F,
		OLED_SET_PRECHARGE_1, 0x11F,
		OLED_SET_PRECHARGE_2, 0x108,
		OLED_SET_VCOMH, 0x107,
		OLED_NORMAL
	};

	Oled_reset();

	Spi_send( &init_seq, (uint32_t)sizeof( init_seq ) / sizeof(uint16_t) );

	Oled_clear();

	Oled_on();
}

/******** Oled_reset *********
* Sends a reset sequence to the ssd1322.
*  Inputs: none
* Outputs: none
*/
void Oled_reset(void)
{

	gpio_clear( PORT_RST, RST );

	Systick_delayTicks(5);

	gpio_set( PORT_RST, RST );

}

/******** Oled_clear *********
* Clears the ssd1322 display memory.
*  Inputs: none
* Outputs: none
*/
void Oled_clear(void)
{
	uint8_t x, y;

	uint16_t clear_seq[] =
	{
		OLED_COL_START_END, 0x100, 0x100,
		OLED_ROW_START_END, 0x100, 0x17F,
		OLED_START_LINE, 0x100,
		OLED_WRITE
	};

	uint16_t data_out = 0x100;

	Spi_send( &clear_seq, (uint32_t)sizeof( clear_seq ) / sizeof(uint16_t) );

	for ( y = 0; y < 32; y++ )
	{
		for ( x = 0; x < 128; x++ )
		{
			Spi_send( &data_out, 1 );
		}
	}
}

/******** Oled_on *********
* Turns the ssd1322 display on.
*  Inputs: none
* Outputs: none
*/
void Oled_on(void)
{
	uint16_t cmd[] = 
	{
		OLED_DISP_ON
	};

	Spi_send( &cmd, 1 );
}

/******** Oled_off *********
* Switches the display matrix off.
*  Inputs: none
* Outputs: none
*/
void Oled_off(void)
{
	uint16_t cmd[] = 
	{
		OLED_DISP_OFF
	};

	Spi_send( &cmd, 1 );
}

/******** Oled_test *********
* Test command sequence.
*  Inputs: none
* Outputs: none
*/
void Oled_test(void)
{
	uint8_t x, y;

	uint16_t clear_seq[] =
	{
		OLED_COL_START_END, 0x100, 0x100,
		OLED_ROW_START_END, 0x100, 0x17F,
		OLED_START_LINE, 0x100,
		OLED_WRITE
	};

	uint16_t data_1 = 0x1F0;
	uint16_t data_2 = 0x10F;

	Spi_send( &clear_seq, (uint32_t)sizeof( clear_seq ) / sizeof(uint16_t) );

	for ( y = 0; y < 32; y++ )
	{
		for ( x = 0; x < 128; x++ )
		{
			if ( ( x % 2 ) == 0 )
			{
				Spi_send( &data_1, 1 );
			}
			else
			{
				Spi_send( &data_2, 1 );
			}
		}
	}
}