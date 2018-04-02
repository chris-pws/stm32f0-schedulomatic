#include "ssd1322_oled.h"

/******** Oled_sendCmd *********
* Sends a single byte command to the display controller.
*  Inputs: a single command byte
* Outputs: none
*/
void Oled_sendCmd( uint8_t cmd )
{
	//uint16_t cmd_shift = cmd >> 1;
	uint16_t cmd_out[] =
	{
		cmd
	};
	//Uart_send( " Z ", 3 );
	Spi_send( &cmd_out, 1 );
}

/******** Oled_sendData *********
* Sends a single byte of data to the display controller.
*  Inputs: a single data byte
* Outputs: none
*/
void Oled_sendData( uint8_t data )
{
	uint16_t data_bit = data & 0x100;
	uint16_t data_out[] =
	{
		data_bit
	};

	Spi_send( &data_out, 1 );
}

/******** Oled_init *********
* Initializes the display controller.
*  Inputs: none
* Outputs: none
*/
void Oled_init(void)
{

	uint16_t init_seq[] = 
	{
		OLED_LOCK_STATUS, 0x112,
		OLED_DISP_OFF,
		OLED_CLOCK_DIV, 0x191,
		OLED_MUX_RATIO, 0x13F,
		//OLED_COL_START_END, 0x11C, 0x15B,
		//OLED_ROW_START_END, 0x100, 0x13F,
		OLED_SET_REMAP, 0x114, 0x111,

		OLED_SET_VDD, 0x101,
		//OLED_MAGIC_1, 0x1A0, 0x1FD,
		OLED_CONTRAST_CURRENT, 0x1FF,
		OLED_MASTER_CONTRAST, 0x10F,
		//OLED_DEFAULT_GRAYTABLE,
		//OLED_EN_GRAY,
		//OLED_MAGIC_2, 0x182, 0x120,
		OLED_SET_PRECHARGE_1, 0x11F,
		OLED_SET_PRECHARGE_2, 0x108,
		OLED_SET_VCOMH, 0x107,
		OLED_NORMAL
	};

	Oled_reset();

	Spi_send( &init_seq, (uint16_t)sizeof( init_seq ) / sizeof(uint16_t) );

	Oled_clear();

	Oled_on();

	//Oled_test();

	//Oled_sendCmd( OLED_INVERSE );
}

/******** Oled_reset *********
* Sends a reset sequence to the ssd1322.
*  Inputs: none
* Outputs: none
*/
void Oled_reset(void)
{

	gpio_clear( PORT_RST, RST );

	Systick_delayTicks(1);

	gpio_set( PORT_RST, RST );
/*
	Systick_delayTicks(500);

	gpio_clear( PORT_RST, RST );

	Systick_delayTicks(500);

	gpio_set( PORT_RST, RST );
*/
}

/******** Oled_clear *********
* Clears the ssd1322 display memory.
*  Inputs: none
* Outputs: none
*/
void Oled_clear(void)
{
	volatile uint16_t x, y;

	uint16_t clear_seq[] =
	{
		OLED_COL_START_END, 0x11C, 0x15B,
		OLED_ROW_START_END, 0x100, 0x13F,
		OLED_START_LINE, 0x100,
		OLED_WRITE
	};

	uint16_t data_out = 0x100;

	Spi_send( &clear_seq, (uint16_t)sizeof( clear_seq ) / sizeof(uint16_t) );

	for ( y = 0; y < 64; y++ )
	{
		for ( x = 0; x < 128; x++ )
		{
	 		Spi_send( &data_out, 1 );
	 		//Systick_delayTicks(12);
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

	Spi_send( &cmd, (uint16_t)sizeof( cmd ) / sizeof(uint16_t) );
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

/******** Oled_invert *********
* Inverts the current contents of the graphic display RAM.
*  Inputs: none
* Outputs: none
*/
void Oled_invert(void)
{
	uint16_t cmd[] = 
	{
		OLED_INVERSE
	};

	Spi_send( &cmd, (uint16_t)sizeof( cmd ) / sizeof(uint16_t) );
}

/******** Oled_normal *********
* Restores the display to normal mode.
*  Inputs: none
* Outputs: none
*/
void Oled_normal(void)
{
	uint16_t cmd[] = 
	{
		OLED_NORMAL
	};

	Spi_send( &cmd, (uint16_t)sizeof( cmd ) / sizeof(uint16_t) );
}

/******** Oled_test *********
* Test command sequence.
*  Inputs: none
* Outputs: none
*/
void Oled_test(void)
{
	volatile uint16_t x, y;
	uint8_t p = 0;

	uint16_t test_seq[] =
	{
		OLED_COL_START_END, 0x11C, 0x15B,
		OLED_ROW_START_END, 0x100, 0x13F,
		OLED_START_LINE, 0x100,
		OLED_WRITE
	};

	uint16_t data_1[] = { 0x112,0x134,0x156,0x178,0x19A,0x1BC,0x1DE,0x1F0 };
	//uint16_t data_2 = 0x16D;
	uint16_t data_t[128];
	for ( x = 0; x < 128; x++ )
	{
		data_t[x] = 0x100;
		
	}

	for ( x = 0; x < 128; x++ )
	{

		data_t[x] = data_1[p];
		p++;
		if ( p > 7 ) p = 0;
	}

	Spi_send( &test_seq, (uint16_t)sizeof( test_seq ) / sizeof(uint16_t) );

	for ( y = 0; y < 64; y++ )
	{

		for ( x = 0; x < 128; x++ )
		{

			Spi_send( &data_t[x], 1 );

		}
	}

	// uint16_t cmd[] = 
	// {
	// 	OLED_VERT_SCROLL, 0x100
	// };
	// uint16_t delay = 10000;
	
	// for ( y = 0; y < 64; y++ )
	// {
		
	// 	Spi_send( &cmd, 2 );
	// 	if ( cmd[1] > 0x163 ) 
	// 	{
	// 		cmd[1] = 0x100;
	// 	}
	// 	cmd[1] += 1;
	// 	Systick_delayTicks(delay);
	// 	delay -= (delay / 16);
	// }
}