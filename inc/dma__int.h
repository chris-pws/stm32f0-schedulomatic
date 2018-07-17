#ifndef DMA__INT_H_

#include <stdint.h>
#include <stdio.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/f0/nvic.h>
#include <libopencm3/stm32/f0/dma.h>
#include <libopencm3/cm3/systick.h>

extern int Flag_DMA_Chan3;
extern int Flag_DMA_Chan4;
extern int systickCount;

// ******* Dma_init *******
// Meta function that initializes the DMA peripheral.
//  Inputs: none
// Outputs: none
void Dma_init(void);

// ******* nvic_init *******
// Initilizes the Nested Vector Interrupt Controller.
//  Inputs: none
// Outputs: none
void nvic_init(void);

// ******* dma_uartTxInit *******
// Sets up DMA transfers between memory and the UART peripheral for 
// transmission.
//  Inputs: none
// Outputs: none
void dma_uartTxInit(void);

// ******* dma_spiTxInit *******
// Sets up DMA transfers between memory and the SPI peripheral for 
// transmission.
//  Inputs: none
// Outputs: none
void dma_spiTxInit(void);

// ******* Sched_flagSignal *******
// Increment semaphore
//  Inputs: pointer to a counting semaphore
// Outputs: none
extern void Sched_flagSignal( int *semaPt );

// ******* Sched_runEventManager *******
// Runs scheduler event manager
//  Inputs: none
// Outputs: none
extern void Sched_runEventManager(void);

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
extern void Uart_send( volatile void* data, int length );

#define DMA__INT_H_ 1
#endif