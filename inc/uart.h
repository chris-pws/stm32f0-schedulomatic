#ifndef UART_H_

#include <stdint.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>

#include "scheduler.h"
#include "queue.h"

/********* Uart_init *******
*  Initializes the USART peripheral for serial communication.
*   Inputs: none
*  Outputs: none
*/
void Uart_init(void);

/********* Uart_fifoTxEvent *******
*  Periodic event that manages the UART transmit queue.
*  Executed from the event scheduler.
*   Inputs: Queue_t pointer, signal flag
*  Outputs: none
*/
void Uart_fifoTxEvent( Queue_t *buffer, int *flagPt );

/********* Uart_dmaTxHandler *******
*  Copies a series of data from a memory address to the serial peripheral DMA
*  transmission channel.
*   Inputs: pointer to a contiguous block of data, the number of bytes.
*  (Depending on MSIZE and PSIZE settings in dma__int).
*  Outputs: none
*/
void Uart_dmaTxHandler( volatile void* data, int length );

/********* Uart_send *******
*  Adds arbitrary number of bytes to the UART transmission buffer.
*   Inputs: pointer to a contiguous block of data, the number of bytes
*  Outputs: none
*/
void Uart_send( volatile void* data, int length );

#define UART_H_ 1
#endif