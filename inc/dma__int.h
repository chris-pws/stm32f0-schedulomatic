#ifndef DMA__INT_H_

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f0/nvic.h>
#include <libopencm3/stm32/f0/dma.h>
#include <libopencm3/cm3/systick.h>

extern int32_t Flag_DMA_Chan4;

extern uint32_t systickCount;

// ******* dma_uartTxInit *******
// Sets up DMA transfers between memory and the serial peripheral for 
// transmission.
// Inputs: none
// Outputs: none
void dma_uartTxInit(void);

// ******* nvic_init *******
// Initilizes the Nested Vector Interrupt Controller.
// Inputs: none
// Outputs: none
void nvic_init(void);

// ******* Sched_flagSignal *******
// Increment semaphore
// Inputs: pointer to a counting semaphore
extern void Sched_flagSignal( int32_t *semaPt );

// ******* Sched_runEventManager *******
// Runs scheduler event manager
// Inputs/Outputs: none
extern void Sched_runEventManager(void);

#define DMA__INT_H_ 1
#endif