#ifndef QUEUE_H_

#include <stdint.h>
#include <stdio.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>

#include "test.h"

#define B_SIZE_FIFO_U8T 128
#define B_SIZE_FIFO_U16T 2048
#define B_SIZE_TEST 8

/******** Queue *********
* Queue superclass
* Provides a base for common queue access methods.
* Child classes inherit these basic functions and can implement their own.
* Queue element format defined by structs queue_format and queue_data.
*/

struct Queue
{
	queue_data *my;

	void (*putFunction)( volatile void *in_buf, Queue_t *me, int length );
	void (*getFunction)( volatile void *out_buf, Queue_t *me, int length );

	int size
	int getIndex; 
	int putIndex;
	int *flagSize;

	void (*handler_function)( volatile void *data, uint8_t length );
	 
};

struct queue_fifo_u8
{
	volatile uint8_t data[B_SIZE_FIFO_U8T];
	 
};

struct queue_fifo_u16
{
	volatile uint16_t data[B_SIZE_FIFO_U16T];
};

/* Defines the available queue structure and data type. This is used once for
*  the constructor function.  i.e. [STRUCTURE_TYPE]
*/
enum queue_format
{ 
	FIFO_U8T = 0, 
	FIFO_U16T 
};

/* Defines the queue data container and links the format parameter.
*/
struct queue_data
{
	enum queue_format type;
	union
	{
		struct queue_fifo_u8 *fifo_u8;
		struct queue_fifo_u16 *fifo_u16;
	} type;
};

typedef struct Queue Queue_t;


/********* Queue_init *******
* Initializes a FIFO queue structure, preparing it for usage.
* Inputs: Pointer to Queue_t, number of elements as size, queue_data pointer,
*         pointer to a constructor function,
*         pointer to a putFunction and a getFunction,
*		  function pointer called by the event scheduler to handle queue
*		  processing.
* Ouputs: None
*/
void Queue_init( Queue_t *me, int size, struct queue_data *my, 
	int *flagSize, 
	void(*initFunction)( Queue_t *me ),
	void(*putFunction)( Queue_t *me, volatile void *in_buf, int length ), 
	void(*getFunction)( Queue_t *me, volatile void *out_buf, int length ),
	void(*handler)( volatile void *data, int length ) );

/********* Queue_put *******
* Public function that appends data to a specified queue based on the
* queue parameter structure provided.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int Queue_put( volatile void *in_buf, Queue_t *me, int length );

/********* Queue_get *******
* Public function that retrieves and removes data from a queue corresponding
* to the supplied queue parameter structure.
*  Inputs: data pointer, Queue_t pointer, and number of elements to
*          read.
* Outputs: number of elements read into the data pointer.
*/
int Queue_get( volatile void *out_buf, Queue_t *me, int length );

/********* queue_fifo_u8_put *******
* Private function that appends data to a supplied queue.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int queue_fifo_u8_put( volatile void *in_buf, Queue_t *me, int length );

/********* queue_fifo_u8_get *******
* Private function to retrieve and remove data from a specified queue.
*  Inputs: data pointer, Queue_t pointer, and number of elements to read
* Outputs: number of elements read into the data pointer.
*/
int queue_fifo_u8_get( volatile void *out_buf, Queue_t *me, int length );

/********* queue_fifo_u16_put *******
* Private function that appends data to a supplied queue.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int queue_fifo_u16_put( volatile void *in_buf, Queue_t *me, int length );

/********* queue_fifo_u16_get *******
* Private function to retrieve and remove data from a specified queue.
*  Inputs: data pointer, Queue_t pointer, and number of elements to read
* Outputs: number of elements read into the data pointer.
*/
int queue_fifo_u16_get( volatile void *out_buf, Queue_t *me, int length );

/********* Queue_flagSizeInit *******
* Initialize a queue size counting flag
*  Inputs: pointer to a queue size flag
* Outputs: none
*/
void Queue_flagSizeInit( int *flagPt );

/********* Queue_flagSizeAdd *******
* Decrement semaphore, blocking task if less than zero
*  Inputs: pointer to a queue size flag, number of elements to add
* Outputs: none
*/
void Queue_flagSizeAdd( int *flagPt, int num_elements );

/********* Queue_flagSizeSub *******
* Subtract  queue size flag
*  Inputs: pointer to a queue size flag, number of elements to substract
* Outputs: none
*/
void Queue_flagSizeSub( int *flagPt, int num_elements );

/********* Uart_send *******
* Adds arbitrary number of bytes to the UART transmission queue.
*  Inputs: pointer to a contiguous block of data, the number of bytes
* Outputs: none
*/
extern void Uart_send( volatile void* data, int length );



#define QUEUE_H_ 1
#endif