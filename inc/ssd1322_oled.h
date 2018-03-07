#ifndef SSD1322_OLED_H_

#include <stdint.h>
#include <stdio.h>
#include "lowlevel.h"
#include "systick.h"

#define OLED_EN_GRAY 0x000
#define OLED_COL_START_END 0x015
#define OLED_WRITE 0x05C
#define OLED_ROW_START_END 0x075
#define OLED_START_LINE 0x0A1
#define OLED_ALL_OFF 0x0A4
#define OLED_ALL_ON 0x0A5
#define OLED_NORMAL 0x0A6
#define OLED_INVERSE 0x0A7
#define OLED_SET_VDD 0x0AB
#define OLED_DISP_OFF 0x0AE
#define OLED_DISP_ON 0x0AF
#define OLED_CLOCK_PHASE 0x0B1
#define OLED_CLOCK_DIV 0x0B3
#define OLED_SET_GAMMA 0x0B8
#define OLED_SET_GRAY 0x0B9
#define OLED_SET_PRECHARGE_1 0x0BB
#define OLED_SET_PRECHARGE_2 0x0B6
#define OLED_SET_VCOMH 0x0BE
#define OLED_CONTRAST_CURRENT 0x0C1
#define OLED_MASTER_CONTRAST 0x0C7

#define OLED_CMD_VDD_INIT 0x1A1

/******** Oled_init *********
* Initializes the display controller.
*  Inputs: none
* Outputs: none
*/
void Oled_init(void);

/******** Oled_reset *********
* Sends a reset sequence to the ssd1322.
*  Inputs: none
* Outputs: none
*/
void Oled_reset(void);

/******** Oled_clear *********
* Clears the ssd1322 display memory.
*  Inputs: none
* Outputs: none
*/
void Oled_clear(void);

/******** Oled_on *********
* Turns the ssd1322 display on.
*  Inputs: none
* Outputs: none
*/
void Oled_on(void);

/******** Oled_off *********
* Switches the display matrix off.
*  Inputs: none
* Outputs: none
*/
void Oled_off(void);

// ******* Spi_send *******
// Adds arbitrary number of elements to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, number of elements to copy
// Outputs: none
extern void Spi_send( volatile void* data, uint32_t length );

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
extern void Uart_send( volatile void* data, uint32_t length );

#define SSD1322_OLED_H_ 1
#endif