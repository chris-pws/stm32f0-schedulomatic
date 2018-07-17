#include "scheduler.h"

/* Create an event table holder
*/

struct sched_eventTable events[NUMEVENTS];

/* Create mutex flags for communication channels
*/

int Flag_DMA_Chan3;
int Flag_DMA_Chan4;
int Flag_test;

/* Instantiate Queue structures
*/

Queue_t Q_fifo_u8_uart;
Queue_t Q_fifo_u8_spi;
Queue_t Q_fifo_u16_test;

/* Create counting flags to track queue sizes
*/

int Flag_queueSize_uart;
int Flag_queueSize_spi;

/* Allocate data stores for queues
*/

volatile uint8_t fifo_uartTxData[B_SIZE_FIFO_U8T];
volatile uint8_t fifo_spiTxData[B_SIZE_FIFO_U16T];
volatile uint16_t fifo_testData[B_SIZE_TEST];

/* Assign data stores to queue_data types
*/

struct queue_fifo_u8 fifo_uartTx =
	{ .data = fifo_uartTxData };

struct queue_fifo_u8 fifo_spiTx =
	{ .data = fifo_spiTxData };

struct queue_fifo_u16 fifo_test =
	{ .data = fifo_testData };

/* Initialize queue_data structs for each queue
*/

struct queue_data fifo_uartTx_data = 
    { .format = FIFO_U8T, .is= { .fifo_u8 = &fifo_uartTx } };

struct queue_data fifo_spiTx_data = 
    { .format = FIFO_U8T, .is= { .fifo_u8 = &fifo_spiTx } };

struct queue_data fifo_test_data = 
    { .format = FIFO_U16T, .is= { .fifo_u16= &fifo_test } };

/* ******* Sched_Init *******
*  Initializes system task fixed rate scheduler.
*/
void Sched_init(void) {

	/* Initialize task communication channel blocking flags */ 
	Sched_flagInit( &Flag_DMA_Chan4, 1 ); 	/*  flag for UART_tx DMA  		*/
	Sched_flagInit( &Flag_DMA_Chan3, 1 ); 	/*  flag for SPI_tx DMA 		*/
	Sched_flagInit( &Flag_test, 1 ); 		/*  test flag for test target	*/

	/* Initialize counting signal reflecting number of elements in queue */ 
	Queue_flagSizeInit( &Flag_queueSize_uart );
	Queue_flagSizeInit( &Flag_queueSize_spi );

	/* Initialize queue size setting */ 
	const int sizeUart = B_SIZE_FIFO_U8T;
	const int sizeSpi = B_SIZE_FIFO_U16T;
	const int sizeTest = B_SIZE_TEST;

	/* Initialize queue_param objects providing: pointer to queue_param_t, 
	*  number of elements, pointer to data array, blocking stored elements
	*  flag, and address of a target callback handler function.
	*/
	Queue_init( &Q_fifo_u8_uart, sizeUart, &fifo_uartTx_data, 
				&Flag_queueSize_uart, 
				queue_fifo_u8_put, queue_fifo_u8_get,
				&Uart_dmaTxHandler );

	Queue_init( &Q_fifo_u8_spi, sizeSpi, &fifo_spiTx_data, 
				&Flag_queueSize_spi,
				queue_fifo_u8_put, queue_fifo_u8_get,
				&Spi_dmaTxHandler );

	Queue_init( &Q_fifo_u16_test, sizeTest, &fifo_test_data, 
				&Flag_test,
				queue_fifo_u16_put, queue_fifo_u16_get,
				&test_handler );

	/* Add events to the task mananger by providing: pointer to event function,
	*  fixed time interval, a queue parameter object, and a target signal flag.
	*/

	Sched_addEvent( &Uart_fifoTxEvent, 25, &Q_fifo_u8_uart, 
				&Flag_DMA_Chan4 );
	/*
	Sched_addEvent( &Spi_fifoTxEvent, 1, &Q_fifo_u8_spi, 
				&Flag_DMA_Chan3 );
	*/
	Sched_addEvent( &test_event, 10000, &Q_fifo_u16_test, &Flag_test );

}

/********* Sched_flagInit *******
*  Initialize a counting semaphore
*   Inputs: pointer to a semaphore
*           initial value of semaphore
*  Outputs: none
*/
void Sched_flagInit( int *flagPt, int value ) 
{
	(*flagPt) = value;

}

/********* Sched_waitFlag *******
*  Decrement semaphore, blocking task if less than zero.
*   Inputs: pointer to a counting semaphore
*  Outputs: none
*/
void Sched_flagWait( int *flagPt ) 
{
	(*flagPt)--;

}

/********* Sched_flagSignal *******
*  Increment semaphore. Value > 0 indicates ready status.
*   Inputs: pointer to a counting semaphore
*  Outputs: none
*/
void Sched_flagSignal( int *flagPt )
{
	(*flagPt)++;
}

/********* Sched_addEvent *******
*  Adds event to event management table
*   Inputs: pointer to a event function
*           period in cycles through the event queue
*           pointer to a fifo type
*  Outputs: none
*/
void Sched_addEvent( 
	void(*function)( Queue_t *queue, int *flagPt ),
	int period_cycles, Queue_t *queue, int *flagPt )
{
	int j;
	for ( j = 0; j < NUMEVENTS; j++ )
	{
		if ( !events[j].eventFunction )
		{
			events[j].eventFunction = function;
			events[j].interval = period_cycles;
			events[j].last = 0;
			events[j].queue = queue;
			events[j].flag = flagPt;
			break;
		}
	}
}

/********* Sched_runEventManager *******
*  Executes functions based on a series of conditions,
*  including elapsed time since last run and busy signals.
*   Inputs: none
*  Outputs: none
*/
void Sched_runEventManager(void)
{
	cm_disable_interrupts();
	uint8_t j;
	uint now, diff;
	
	for ( j = 0; j < NUMEVENTS; j++ )
	{
		now = Systick_timeGetCount();

		/* Number of program execution cycles since last execution.
		*/
		diff = Systick_timeDelta( events[j].last, now );

		/* if: reception conditions are true (at or past interval delta, 
		*  task flag > 0, queue size > 0), run event
		*/
		if ( ( diff >= events[j].interval ) && ( (*events[j].flag) ) 
				&& ( (*events[j].queue->flagSize) ) )  
		{
			events[j].eventFunction( events[j].queue, events[j].flag );
			events[j].last = now;

		}			
		
	}
	
	cm_enable_interrupts();
}

void test_event( Queue_t *queue, int *flagPt )
{
	gpio_toggle(GPIOB, GPIO8);
}

void test_handler( volatile void* data, int length )
{
	return;
}