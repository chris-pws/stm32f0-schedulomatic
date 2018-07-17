#ifndef SPI_H_

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/rcc.h>

#include "lowlevel.h"
#include "scheduler.h"
#include "queue.h"
#include "dma__int.h"

// ******* Spi_init *******
// Initializes the SPI peripheral for simplex serial transmission.
// 9 bits per word
//  Inputs: none
// Outputs: none
void Spi_init(void);

// ******* Spi_fifoTxEvent *******
// Periodic event that manages the SPI transmission queue.
// Executed from the event scheduler.
//  Inputs: Queue_t pointer, signal flag
// Outputs: none
void Spi_fifoTxEvent( Queue_t *buffer, int *flagPt );

// ******* Spi_dmaTxHandler *******
// Copies data from a memory address to the SPI peripheral DMA transmission 
// channel.
//  Inputs: pointer to a contiguous block of data, number of elements to copy
// Outputs: none
void Spi_dmaTxHandler( volatile void* data, int length );

// ******* Spi_send *******
// Adds arbitrary number of elements to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, number of elements to copy
// Outputs: none
void Spi_send( volatile void* data, int length );

// ******* Spi_enableNssPulse *******
// Enables SPI to generate an NSS pulse between two consecutive words while
// performing consecutive transfers. The NSS is held high when transfers are
// complete. Applies only to master mode, and has no effect when FRF or
// CPHA control registers are 1.
//  Inputs: SPI peripheral identifier
// Outputs: none
void Spi_enableNssPulse( int spi );

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
extern void Uart_send( volatile void* data, int length );

#define SPI_H_ 1
#endif