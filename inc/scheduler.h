#ifndef SCHEDULER_H_
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/f0/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/cortex.h>

#include "systick.h"
#include "queue.h"

#define NUMEVENTS 2

/* Data transfer blocking flags. */
extern int Flag_DMA_Chan3;
extern int Flag_DMA_Chan4;
extern int Flag_test;

/* Buffer parameter initialization exports to global. */
extern Queue_t Q_fifo_u8_uart;
extern Queue_t Q_fifo_u8_spi;

/* Scheduler event table. */
struct sched_eventTable {
	                    /* Event function is executed in the event scheduler loop.  */
	void(*eventFunction)( Queue_t *queue, int *flagPt ); 
	Queue_t *queue;     /* pointer to data queue related to the particular event.   */
	int *flag;          /* pointer to an initialized semaphore for event signaling. */
	int interval;       /* how often the manager function will be called.           */
	int last;           /* time of last execution.                                  */
};


/********* Sched_Init *******
*  Initializes event scheduling manager
*   Inputs: none
*  Outputs: none
*/
void Sched_init(void);

/********* Sched_flagInit *******
*  Initialize a counting semaphore
*   Inputs: pointer to a semaphore
*           initial value of semaphore
*  Outputs: none
*/
void Sched_flagInit( int *semaPt, int value );

/********* Sched_flagWait *******
*  Decrement semaphore, blocking task if less than zero
*   Inputs: pointer to a counting semaphore
*  Outputs: none
*/
void Sched_flagWait( int *semaPt );

/********* Sched_flagSignal *******
*  Increment semaphore
*   Inputs: pointer to a counting semaphore
*  Outputs: none
*/
void Sched_flagSignal( int *semaPt );

/********* Sched_addEvent *******
*  Adds event to event management table
*   Inputs: pointer to a event function
*           period in milliseconds
*           pointer to a Queue type
*  Outputs: none
*/
void Sched_addEvent( 
	void(*function)( Queue_t *queue, int *flagPt ),
	int period_cycles, Queue_t *queue, int *flagPt );

/********* Sched_runEventManager *******
*  Runs scheduler event manager
*   Inputs: none
*  Outputs: none
*/
void Sched_runEventManager(void);

/* External functions */

/********* Uart_fifoTxEvent *******
*  Periodic event that manages the UART transmit queue.
*   Inputs: fifo_t pointer, signal flag
*  Outputs: none
*/
extern void Uart_fifoTxEvent( Queue_t *queue, int *flagPt );

/********* Uart_dmaTxHandler *******
*  Copies a series of data from a memory address to the serial peripheral DMA
*  transmission channel.
*   Inputs: pointer to a contiguous block of data, the number of bytes to copy
*  Outputs: none
*/
extern void Uart_dmaTxHandler( volatile void* data, int length );

/********* Spi_fifoTxEvent *******
*  Periodic event that manages the SPI transmission queue.
*  Executed from the event scheduler.
*   Inputs: buffer_fifo_t pointer, signal flag
*  Outputs: none
*/
extern void Spi_fifoTxEvent( Queue_t *queue, int *flagPt );

/********* Spi_dmaTxHandler *******
*  Copies data from a memory address to the SPI peripheral DMA transmission 
*  channel.
*   Inputs: pointer to a contiguous block of data, the number of bytes to copy
*  Outputs: none
*/
extern void Spi_dmaTxHandler( volatile void* data, int length );

/* Test functions */

void test_event( Queue_t *queue, int *flagPt );

void test_handler( volatile void* data, int length );


#define SCHEDULER_H_ 1
#endif