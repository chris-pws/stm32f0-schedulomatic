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
void Spi_fifoTxEvent( buffer_param_t *buffer, int32_t *flagPt );

// ******* Spi_dmaTxHandler *******
// Copies data from a memory address to the SPI peripheral DMA transmission 
// channel.
//  Inputs: pointer to a contiguous block of data, the number of bytes to copy
// Outputs: none
void Spi_dmaTxHandler( volatile void* data, uint8_t length );

// ******* Spi_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: 
// Outputs: none
void Spi_send( volatile void* data, uint32_t length );

// ******* Spi_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: 
// Outputs: none
void Spi_send( volatile void* data, uint32_t length );

// ******* Spi_csnHigh *******
// Sets the software controlled chip select line high.
//  Inputs: 
// Outputs: none
void Spi_csnHigh(void);

// ******* Spi_csnLow *******
// Sets the software controlled chip select line low.
//  Inputs: none
// Outputs: none
void Spi_csnLow(void);

// ******* Spi_begin *******
// Called at the beginning of an SPI word transfer.
//  Inputs: none
// Outputs: none
void Spi_begin(void);

// ******* Spi_end *******
// Called at the end of an SPI word transfer.
//  Inputs: none
// Outputs: none
void Spi_end(void);

#define SPI_H_ 1
#endif