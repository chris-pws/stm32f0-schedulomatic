#ifndef SCHEDULER_H_
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/f0/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/cortex.h>

#include "systick.h"
#include "buffer.h"

#define NUMEVENTS 2

// Signaling flags
extern int32_t Flag_DMA_Chan4;
extern int32_t Flag_test;

extern buffer_fifo_t fifo_uartTx[1];

// Scheduler event management table
struct eventTable_t {
	// event function is executed in the event scheduler loop
	void(*eventFunction)( buffer_fifo_t*, int32_t *flagPt ); 
	buffer_fifo_t *buffer; // pointer to data queue related to the particular event.
	int32_t *flag; // pointer to an initialized semaphore for event signaling.
	uint32_t interval; // how often the manager function will be called.
	uint32_t last; // time of last execution.
};
typedef struct eventTable_t eventTableType;

// ******* Sched_Init *******
// Initializes event scheduling manager
// Inputs/Ouputs: none
void Sched_Init(void);

// ******* Sched_flagInit *******
// Initialize a counting semaphore
// Inputs: pointer to a semaphore
//         initial value of semaphore
// Outputs: none
void Sched_flagInit( int32_t *semaPt, int32_t value );

// ******* Sched_flagWait *******
// Decrement semaphore, blocking task if less than zero
// Inputs: pointer to a counting semaphore
void Sched_flagWait( int32_t *semaPt );

// ******* Sched_flagSignal *******
// Increment semaphore
// Inputs: pointer to a counting semaphore
void Sched_flagSignal( int32_t *semaPt );

// ******* Sched_addEvent *******
// Adds event to event management table
// Inputs: pointer to a event function
//         period in milliseconds
//         pointer to a fifo type
// Outputs: nothing
void Sched_addEvent( void(*function)( buffer_fifo_t*, int32_t *flagPt ), uint32_t period_cycles, buffer_fifo_t *buffer, int32_t *flagPt );

// ******* Sched_runEventManager *******
// Runs scheduler event manager
// Inputs/Outputs: none
void Sched_runEventManager(void);

// ******* Buffer_fifoTxEvent *******
// Periodic event that manages transmit queue.
// Inputs: fifo_t pointer, signal flag
// Outputs: none
extern void Buffer_fifoTxEvent( buffer_fifo_t *buffer, int32_t *flagPt );

// ******* Uart_dmaTxHandler *******
// Copies a series of data from a memory address to the serial peripheral DMA
// transmission channel.
// Inputs: pointer to a contiguous block of data, the number of bytes to copy
// up to a maximum of 8 bytes.
// Outputs: none
extern void Uart_dmaTxHandler( volatile void* data, uint8_t length );

void test_process(buffer_fifo_t *buffer, int32_t *flagPt );

#define SCHEDULER_H_ 1
#endif