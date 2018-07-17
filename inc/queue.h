#ifndef QUEUE_H_

#include <stdint.h>
#include <stdio.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>

#include "test.h"

#define B_SIZE_FIFO_U8T 128
#define B_SIZE_FIFO_U16T 2048
#define B_SIZE_TEST 8

struct queue_fifo_u8
{
	volatile uint8_t *data;
	 
};

struct queue_fifo_u16
{
	volatile uint16_t *data;
};

/* Defines the available queue structure and data type.
*/
enum queue_format
{ 
	/* each data type corresponds to a struct pointer in queue_data.is */
	FIFO_U8T = 0, 
	FIFO_U16T 

};

/* Defines the queue data container and links the format parameter.
*/
struct queue_data
{
	enum queue_format format;
	union
	{
		struct queue_fifo_u8 *fifo_u8;
		struct queue_fifo_u16 *fifo_u16;

	} is;
};

/******** Queue *********
* Queue superclass
* Provides a base for common queue access methods.
* Child classes inherit these basic functions and can implement their own.
* Queue element format defined by structs queue_format and queue_data.
*/

typedef struct Queue Queue_t;

struct Queue
{
	/* queue_data container allows for different queue element types */
	struct queue_data *queue;

	/* queue manipulation functions */
	int (*putFunction)( Queue_t *me, volatile void *in_buf, int length );
	int (*getFunction)( Queue_t *me, volatile void *out_buf, int length );

	/* universal attributes */
	int size;
	int getIndex; 
	int putIndex;
	int *flagSize;

	/* queue callback function handles data transfer from the scheduler */
	void (*handler_function)( volatile void *data, int length );
	 
};


/********* Queue_init *******
* Initializes a queue structure, preparing it for usage.
* Inputs: Pointer to Queue_t, number of elements as size, queue_data pointer,
*         pointer to a putFunction and a getFunction,
*		  function pointer callback to handle queue xfer from the scheduler.
* Ouputs: None
*/
void Queue_init( Queue_t *me, int size, struct queue_data *data, 
	int *flagSize, 
	int(*putFunction)( Queue_t *me, volatile void *in_buf, int length ), 
	int(*getFunction)( Queue_t *me, volatile void *out_buf, int length ),
	void(*handler)( volatile void *data, int length ) );

/********* Queue_put *******
* Public function that appends data to a specified queue based on the
* queue parameter structure provided.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int Queue_put( Queue_t *me, volatile void *in_buf, int length );

/********* Queue_get *******
* Public function that retrieves and removes data from a queue corresponding
* to the supplied queue parameter structure.
*  Inputs: data pointer, Queue_t pointer, and number of elements to
*          read.
* Outputs: number of elements read into the data pointer.
*/
int Queue_get( Queue_t *me, volatile void *out_buf, int length );

/********* queue_fifo_u8_init *******
* Private constructor function that initializes the provided Queue_t type with
* the queue_data specific to the fifo_u8 queue type.
*  Inputs: pointer to Queue_t, pointer to queue_data struct
* Outputs: none
*/
void queue_fifo_u8_init( Queue_t *me, struct queue_data *my );

/********* queue_fifo_u8_put *******
* Private function that appends data to a supplied queue.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int queue_fifo_u8_put( Queue_t *me, volatile void *in_buf, int length );

/********* queue_fifo_u8_get *******
* Private function to retrieve and remove data from a specified queue.
*  Inputs: data pointer, Queue_t pointer, and number of elements to read
* Outputs: number of elements read into the data pointer.
*/
int queue_fifo_u8_get( Queue_t *me, volatile void *out_buf, int length );

/********* queue_fifo_u16_init *******
* Private constructor function that initializes the provided Queue_t type with
* the queue_data specific to the fifo_u16 queue type.
*  Inputs: pointer to Queue_t, pointer to queue_data struct
* Outputs: none
*/
void queue_fifo_u16_init( Queue_t *me, struct queue_data *my );

/********* queue_fifo_u16_put *******
* Private function that appends data to a supplied queue.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int queue_fifo_u16_put( Queue_t *me, volatile void *in_buf, int length );

/********* queue_fifo_u16_get *******
* Private function to retrieve and remove data from a specified queue.
*  Inputs: data pointer, Queue_t pointer, and number of elements to read
* Outputs: number of elements read into the data pointer.
*/
int queue_fifo_u16_get( Queue_t *me, volatile void *out_buf, int length );

/********* Queue_flagSizeInit *******
* Initialize a queue size counting flag
*  Inputs: pointer to a queue size flag
* Outputs: none
*/
void Queue_flagSizeInit( int *flagPt );

/********* Queue_flagSizeAdd *******
* Add a number to the counting flag reflecting number of elements in queue
*  Inputs: pointer to a queue size flag, number of elements to add
* Outputs: none
*/
void Queue_flagSizeAdd( int *flagPt, int num_elements );

/********* Queue_flagSizeSub *******
* Subtract from the counting flag, blocking queue-related tasks if == 0
*  Inputs: pointer to a queue size flag, number of elements to substract
* Outputs: none
*/
void Queue_flagSizeSub( int *flagPt, int num_elements );

/********* Uart_send *******
* Adds arbitrary number of bytes to the UART transmission queue.
*  Inputs: pointer to a contiguous block of data, the number of bytes to read
* Outputs: none
*/
extern void Uart_send( volatile void* data, int length );


#define QUEUE_H_ 1
#endif