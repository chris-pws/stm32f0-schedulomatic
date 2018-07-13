#include "queue.h"

// ******* Queue_Init *******
// Initializes a queue parameter structure, preparing it for usage.
// Inputs: Pointer to queue_param_t.
//		   Function pointer called by the event scheduler to handle queue 
//		   processing.
// Ouputs: None
void Queue_init( Queue_t *me, int size, struct queue_data *my, 
	int *flagSize, 
	void(*initFunction)( Queue_t *me ),
	void(*putFunction)( Queue_t *me, volatile void *in_buf, int length ), 
	void(*getFunction)( Queue_t *me, volatile void *out_buf, int length ),
	void(*handler)( volatile void *data, int length ) )
{
	switch ( queue->type )
	{
		case FIFO_U8T:
		{
			queue->is.fifo_u8->data = data;
			queue->is.fifo_u8->getIndex = 0;
			queue->is.fifo_u8->putIndex = 0;
			queue->is.fifo_u8->size = size;
			queue->is.fifo_u8->handler_function = handler;
			queue->flagSize = flagSize;

		}

		case FIFO_U16T:
		{
			queue->is.fifo_u16->getPt = (uint16_t*)&queue->is.fifo_u16->data[0];
			queue->is.fifo_u16->putPt = (uint16_t*)&queue->is.fifo_u16->data[0];
			queue->is.fifo_u16->handler_function = handler;
			queue->flagSize = flagSize;

		}
	}

}

// ******* Queue_put *******
// Public function that appends data to a specified queue based on the
// queue parameter structure provided.
//  Inputs: pointer to data, pointer to a queue_param_t, data length
// Outputs: number of elements inserted successfully
int Queue_put( volatile void *in_buf, Queue_t *me, int length )
{
	uint16_t num_queued;

	cm_disable_interrupts();
	/*
	*  Call the specialized helper function corresponding to the supplied 
	*  queue parameters.
	*/
	switch ( queue->type )
	{

		case FIFO_U8T:
			num_queued = queue_fifo_u8_put( in_buf, queue->is.fifo_u8, length );
			Queue_flagSizeAdd( queue->flagSize, num_queued );
			cm_enable_interrupts();
			return num_queued; // return number of data added to the queue
			break;


		case FIFO_U16T:
			num_queued = queue_fifo_u16_put( in_buf, queue->is.fifo_u16, length );
			Queue_flagSizeAdd( queue->flagSize, num_queued );
			//Test_in( &a_test_table, num_queued );
			cm_enable_interrupts();
			return num_queued; // return number of data added to the queue
			break;
	}

	cm_enable_interrupts();
	return 0;
}

// ******* Queue_get *******
// Public function that retrieves and removes data from a queue corresponding
// to the supplied queue parameter structure.
//  Inputs: data pointer, queue_param_t pointer, and number of elements to
//          read.
// Outputs: number of elements read into the data pointer.
int Queue_get( volatile void *out_buf, Queue_t *me, int length )
{
	uint16_t num_read;

	cm_disable_interrupts();
	/*
	*  Call the specialized helper function corresponding to the supplied 
	*  queue parameters.
	*/
	switch ( queue->type )
	{

		case FIFO_U8T:
			num_read = queue_fifo_u8_get( out_buf, queue->is.fifo_u8, length );
			Queue_flagSizeSub( queue->flagSize, num_read );
			cm_enable_interrupts();
			return num_read;
			break;

		case FIFO_U16T:

			num_read = queue_fifo_u16_get( out_buf, queue->is.fifo_u16, length );
			Queue_flagSizeSub( queue->flagSize, num_read );
			//Test_out( &a_test_table, num_read );
			cm_enable_interrupts();
			return num_read;
			break;

	}

	cm_enable_interrupts();
	return 0;
}

// ******* queue_fifo_u8_put *******
// Private function that appends data to a supplied queue.
//  Inputs: pointer to data, pointer to a queue_fifo_t, data length
// Outputs: number of elements inserted successfully
int queue_fifo_u8_put( volatile void *in_buf, Queue_t *me, int length )
{

	uint16_t j;
	uint16_t nextPutIndex;
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

		nextPutIndex = b_u8t->putIndex + 1;

		if ( nextPutIndex == ( b_u8t->size - 1 ) )
		{
			nextPutIndex = 0;
		}
		
		if ( nextPutIndex == b_u8t->getIndex )
		{
			return j; // no vacancy, return number of elements fetched
		}

		b_u8t->data[b_u8t->putIndex] = *p++;
		// Set putIndex to the next element
		b_u8t->putIndex = nextPutIndex;
		
	}
	return j; // return number of data added to the queue
}

// ******* queue_fifo_u8_get *******
// Private function to retrieve and remove data from a specified queue.
//  Inputs: data pointer, queue_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
int queue_fifo_u8_get( volatile void *out_buf, Queue_t *me, int length )
{

	uint16_t j;
	volatile uint8_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) 
	{	
		// Check for get & put index collision
		if ( b_u8t->getIndex != b_u8t->putIndex )
		{
			*p++ = b_u8t->data[b_u8t->getIndex];

			b_u8t->getIndex += 1;

			// Check for index wrap-around
			if ( b_u8t->getIndex == ( b_u8t->size - 1 ) )
			{
				b_u8t->getIndex = 0;
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
int queue_fifo_u16_put( volatile void *in_buf, Queue_t *me, int length )
{

	uint16_t j;
	uint16_t *nextPutPt;

	volatile uint16_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for queue overflow
		*  First handle wrapping if we've reached the queuesize. Check to see
		*  if there's space in the queue: full condition reached when the
		*  advancing putPt meets the getPt. 
		*/ 

		nextPutPt = (uint16_t*)( b_u16t->putPt + 1 );

		if ( nextPutPt == (uint16_t*)&b_u16t->data[B_SIZE_FIFO_U16T] ) 
		{
			nextPutPt = (uint16_t*)&b_u16t->data[0];
		}
		
		if ( nextPutPt == b_u16t->getPt )
		{

			Uart_send( "x", 1 );
			return j; // no vacancy, return number of elements fetched
		}

		(*b_u16t->putPt) = *p++;
		// Set putPt address to the next element
		b_u16t->putPt = nextPutPt;
		
	}

	return j; // return number of data added to the queue
}

// ******* queue_fifo_u16_get *******
// Private function to retrieve and remove data from a specified queue.
//  Inputs: data pointer, queue_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
int queue_fifo_u16_get( volatile void *out_buf, Queue_t *me, int length )
{

	uint16_t j;
	volatile uint16_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) 
	{	
		// Check for get & put pointer collision
		if ( b_u16t->getPt != b_u16t->putPt )
		{

			*p++ = (*b_u16t->getPt);

			b_u16t->getPt = (uint16_t*)&b_u16t->getPt[1];

			// Check for pointer wrap-around
			if ( b_u16t->getPt == (uint16_t*)&b_u16t->data[B_SIZE_FIFO_U16T] )
			{
				b_u16t->getPt = (uint16_t*)&b_u16t->data[0];
			}

		}
		else
		{
			// Nothing left, return number of elements retrieved.
			Uart_send( "j", 1 );
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