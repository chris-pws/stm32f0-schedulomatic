#ifndef UART_H_

#include <stdint.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>

#include "scheduler.h"

// ******* Uart_init *******
// Initializes the USART peripheral for serial communication.
// Inputs: none
// Outputs: none
void Uart_init(void);

// ******* Uart_dmaTxHandler *******
// Copies a series of data from a memory address to the serial peripheral DMA
// transmission channel.
// Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
void Uart_dmaTxHandler( volatile void* data, uint8_t length );

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
// Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
void Uart_send( volatile void* data, uint32_t length );

#define UART_H_ 1
#endif