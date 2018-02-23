#ifndef SPI_H_

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>

#include "lowlevel.h"

// ******* Spi_init *******
// Initializes the SPI peripheral for simplex serial transmission.
// 9 bits per word
//  Inputs: none
// Outputs: none
void Spi_init(void);

// ******* Spi_fifoTxEvent *******
// Periodic event that manages the SPI transmission queue.
// Executed from the event scheduler.
//  Inputs: buffer_fifo_t pointer, signal flag
// Outputs: none
void Spi_fifoTxEvent( buffer_fifo_t *buffer, int32_t *flagPt );

// ******* Spi_dmaTxHandler *******
// Copies data from a memory address to the SPI peripheral DMA transmission 
// channel.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
void Spi_dmaTxHandler( volatile void* data, uint8_t length );

// ******* Spi_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
void Spi_send( volatile void* data, uint32_t length );

#define SPI_H_ 1
#endif