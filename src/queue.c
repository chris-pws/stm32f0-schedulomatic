#include "queue.h"

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
	void(*handler)( volatile void *data, int length ) )
{
	me->size = size;
	me->flagSize = flagSize;
	me->handler_function = handler;

	me->queue = data;
	me->getIndex = 0;
	me->putIndex = 0;

	me->putFunction = putFunction;
	me->getFunction = getFunction;
}

/********* Queue_put *******
* Public function that appends data to a specified queue based on the
* queue parameter structure provided.
*  Inputs: pointer to data, pointer to a Queue_t, data length
* Outputs: number of elements inserted successfully
*/
int Queue_put( Queue_t *me, volatile void *in_buf, int length )
{
	int num_queued;

	cm_disable_interrupts();
	
	/*
	*  Call the specialized Queue_put function as assigned in Queue_init()
	*/

	num_queued = me->putFunction( me, in_buf, length );

	/*
	*  Add the number of elements successfully appended to the queue.
	*/

	Queue_flagSizeAdd( me->flagSize, num_queued );

	cm_enable_interrupts();

	return num_queued;
}

// ******* Queue_get *******
// Public function that retrieves and removes data from a queue corresponding
// to the supplied queue parameter structure.
//  Inputs: data pointer, Queue_t pointer, and number of elements to
//          read.
// Outputs: number of elements read into the data pointer.
int Queue_get( Queue_t *me, volatile void *out_buf, int length )
{
	int num_read;

	cm_disable_interrupts();

	/*
	*  Call the specialized Queue_get function as assigned in Queue_init().
	*/

	num_read = me->getFunction( me, out_buf, length );

	/*
	*  Subtract the number of elements successfully read from the queue.
	*/

	Queue_flagSizeSub( me->flagSize, num_read );

	cm_enable_interrupts();

	return num_read;
}

// ******* queue_fifo_u8_put *******
// Private function that appends data to a supplied queue.
//  Inputs: pointer to data, pointer to a queue_fifo_t, data length
// Outputs: number of elements inserted successfully
int queue_fifo_u8_put( Queue_t *me, volatile void *in_buf, int length )
{

	int j;
	int nextPutIndex;
	volatile uint8_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for queue overflow
		*  First handle wrapping if we've reached the queue size. Check to see
		*  if there's space in the queue: full condition reached when the
		*  advancing putIndex meets the getIndex. We avoid corrupting the queue
		*  by writing to its index variables only after this test passes.
		*/ 

		nextPutIndex = me->putIndex + 1;

		if ( nextPutIndex == ( me->size - 1 ) )
		{
			nextPutIndex = 0;
		}
		
		if ( nextPutIndex == me->getIndex )
		{
			return j; // no vacancy, return number of elements fetched so far
		}

		me->queue->is.fifo_u8->data[me->putIndex] = *p++;
		// Set putIndex to the next element
		me->putIndex = nextPutIndex;
		
	}
	return j; // return number of data added to the queue
}

// ******* queue_fifo_u8_get *******
// Private function to retrieve and remove data from a specified queue.
//  Inputs: data pointer, queue_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
int queue_fifo_u8_get( Queue_t *me, volatile void *out_buf, int length )
{

	int j;
	volatile uint8_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) 
	{	
		// Check for get & put index collision == empty condition
		if ( me->getIndex != me->putIndex )
		{
			*p++ = me->queue->is.fifo_u8->data[me->getIndex];

			me->getIndex += 1;

			// Check for index wrap-around
			if ( me->getIndex == ( me->size - 1 ) )
			{
				me->getIndex = 0;
			}

		}
		else
		{

			// Nothing left, return number of elements retrieved.
			return j;
		}
	}

	return j;
}

// ******* queue_fifo_u16_put *******
// Private function that appends data to a supplied queue.
//  Inputs: pointer to data, pointer to a queue_fifo_t, data length
// Outputs: number of elements inserted successfully
int queue_fifo_u16_put( Queue_t *me, volatile void *in_buf, int length )
{

	int j;
	int nextPutIndex;

	volatile uint16_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for queue overflow
		*  First handle wrapping if we've reached the queue size. Check to see
		*  if there's space in the queue: full condition reached when the
		*  advancing putIndex meets the getIndex. We avoid corrupting the queue
		*  by writing to its index variables only after this test passes.
		*/ 

		nextPutIndex = me->putIndex + 1;

		if ( nextPutIndex == ( me->size - 1 ) )
		{
			nextPutIndex = 0;
		}
		
		if ( nextPutIndex == me->getIndex )
		{
			return j; // no vacancy, return number of elements fetched so far
		}

		me->queue->is.fifo_u16->data[me->putIndex] = *p++;
		// Set putIndex to the next element
		me->putIndex = nextPutIndex;
		
	}
	return j; // return number of data added to the queue
}

// ******* queue_fifo_u16_get *******
// Private function to retrieve and remove data from a specified queue.
//  Inputs: data pointer, queue_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
int queue_fifo_u16_get( Queue_t *me, volatile void *out_buf, int length )
{

	uint16_t j;
	volatile uint16_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) 
	{	
		// Check for get & put index collision == empty condition
		if ( me->getIndex != me->putIndex )
		{
			*p++ = me->queue->is.fifo_u16->data[me->getIndex];

			me->getIndex += 1;

			// Check for index wrap-around
			if ( me->getIndex == ( me->size - 1 ) )
			{
				me->getIndex = 0;
			}

		}
		else
		{

			// Nothing left, return number of elements retrieved.
			return j;
		}
	}

	return j;
}

// ******* Queue_flagSizeInit *******
// Initialize a queue size counting flag
//  Inputs: pointer to a queue size flag
// Outputs: none
void Queue_flagSizeInit( int *flagPt )
{

	(*flagPt) = 0;

}


// ******* Queue_flagSizeAdd *******
// Decrement semaphore, blocking task if less than zero
//  Inputs: pointer to a flag, number of elements to add
// Outputs: none
void Queue_flagSizeAdd( int *flagPt, int num_elements )
{

	(*flagPt) += num_elements;

}

// ******* Queue_flagSizeSub *******
// Subtract  queue size flag
//  Inputs: pointer to a flag, number of elements to substract
// Outputs: none
void Queue_flagSizeSub( int *flagPt, int num_elements )
{

	(*flagPt) -= num_elements;

	if ( (*flagPt) < 0 )
	{
		(*flagPt) = 0;
	}

}