#include "buffer.h"

// ******* Buffer_Init *******
// Initializes a buffer parameter structure, preparing it for usage.
// Inputs: Pointer to buffer_param_t.
//		   Function pointer called by the event scheduler to handle queue processing.
// Ouputs: None
void Buffer_init( buffer_param_t *buffer, 
	void(*handler)( volatile void *data, uint8_t length ) )
{
	switch ( buffer->type )
	{
		case FIFO_U8T:
		{
			buffer->is.fifo_u8->getPt = (uint8_t*)&buffer->is.fifo_u8->data[0];
			buffer->is.fifo_u8->putPt = (uint8_t*)&buffer->is.fifo_u8->data[0];
			buffer->is.fifo_u8->handler_function = handler;

		}

		case FIFO_U16T:
		{
			buffer->is.fifo_u16->getPt = (uint16_t*)&buffer->is.fifo_u16->data[0];
			buffer->is.fifo_u16->putPt = (uint16_t*)&buffer->is.fifo_u16->data[0];
			buffer->is.fifo_u16->handler_function = handler;

		}
	}

}

// ******* Buffer_put *******
// Public function that appends data to a specified buffer based on the
// buffer parameter structure provided.
//  Inputs: pointer to data, pointer to a buffer_param_t, data length
// Outputs: number of elements inserted successfully
uint16_t Buffer_put( volatile void *in_buf, 
	buffer_param_t *buffer, 
	uint16_t length )
{
	uint16_t num_queued;

	/*
	*  Call the specialized helper function corresponding to the supplied 
	*  buffer parameters.
	*/
	switch ( buffer->type )
	{

		case FIFO_U8T:
			num_queued = buffer_fifo_u8_put( in_buf, buffer->is.fifo_u8, length );
			return num_queued; // return number of data added to the buffer
			break;


		case FIFO_U16T:
			num_queued = buffer_fifo_u16_put( in_buf, buffer->is.fifo_u16, length );
			return num_queued; // return number of data added to the buffer
			break;
	}

	return 0;
}

// ******* Buffer_get *******
// Public function that retrieves and removes data from a buffer corresponding
// to the supplied buffer parameter structure.
//  Inputs: data pointer, buffer_param_t pointer, and number of elements to
//          read.
// Outputs: number of elements read into the data pointer.
uint16_t Buffer_get( volatile void *out_buf, 
	buffer_param_t *buffer, 
	uint16_t length )
{
	uint16_t num_read;

	/*
	*  Call the specialized helper function corresponding to the supplied 
	*  buffer parameters.
	*/
	switch ( buffer->type )
	{

		case FIFO_U8T:
			num_read = buffer_fifo_u8_get( out_buf, buffer->is.fifo_u8, length );

			return num_read;
			break;

		case FIFO_U16T:

			num_read = buffer_fifo_u16_get( out_buf, buffer->is.fifo_u16, length );

			return num_read;
			break;

	}
	return 0;
}

// ******* buffer_fifo_u8_put *******
// Private function that appends data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of elements inserted successfully
uint16_t buffer_fifo_u8_put( volatile void *in_buf, 
	struct buffer_fifo_u8 *b_u8t, 
	uint16_t length )
{
	cm_disable_interrupts();
	uint16_t j;
	uint8_t *nextPutPt;
	volatile uint8_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for buffer overflow
		*  First handle wrapping if we've reached the buffersize. Check to see
		*  if there's space in the queue: full condition reached when the
		*  advancing putPt meets the getPt. 
		*/ 

		nextPutPt = (uint8_t*)( b_u8t->putPt + 1 );

		if ( nextPutPt == (uint8_t*)&b_u8t->data[B_SIZE_FIFO_U8T] ) 
		{
			nextPutPt = (uint8_t*)&b_u8t->data[0];
		}
		
		if ( nextPutPt == b_u8t->getPt )
		{
			cm_enable_interrupts();
			return j; // no vacancy, return number of elements fetched
		}

		(*b_u8t->putPt) = *p++;
		// Set putPt address to the next element
		b_u8t->putPt = nextPutPt;
		
	}
	cm_enable_interrupts();
	return j; // return number of data added to the buffer
}

// ******* buffer_fifo_u8_get *******
// Private function to retrieve and remove data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
uint16_t buffer_fifo_u8_get( volatile void *out_buf, 
	struct buffer_fifo_u8 *b_u8t, 
	uint16_t length )
{
	cm_disable_interrupts();
	uint16_t j;
	volatile uint8_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) {
		
		// Check for get & put pointer collision
		if ( b_u8t->getPt != b_u8t->putPt )
		{
			*p++ = (*b_u8t->getPt);

			b_u8t->getPt = (uint8_t*)&b_u8t->getPt[1];

			// Check for pointer wrap-around
			if ( b_u8t->getPt == (uint8_t*)&b_u8t->data[B_SIZE_FIFO_U8T] )
			{
				b_u8t->getPt = (uint8_t*)&b_u8t->data[0];
			}

		}
		else
		{
			cm_enable_interrupts();
			// Nothing left, return number of elements retrieved.
			return j;
		}
	}
	cm_enable_interrupts();
	return j;
}

// ******* buffer_fifo_u16_put *******
// Private function that appends data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of elements inserted successfully
uint16_t buffer_fifo_u16_put( volatile void *in_buf, 
	struct buffer_fifo_u16 *b_u16t, 
	uint16_t length )
{
	cm_disable_interrupts();
	uint16_t j;
	uint16_t *nextPutPt;

	volatile uint16_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for buffer overflow
		*  First handle wrapping if we've reached the buffersize. Check to see
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
			cm_enable_interrupts();
			Uart_send( "x", 1 );
			return j; // no vacancy, return number of elements fetched
		}

		(*b_u16t->putPt) = *p++;
		// Set putPt address to the next element
		b_u16t->putPt = nextPutPt;
		
	}
	cm_enable_interrupts();
	return j; // return number of data added to the queue
}

// ******* buffer_fifo_u16_get *******
// Private function to retrieve and remove data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
uint16_t buffer_fifo_u16_get( volatile void *out_buf, 
	struct buffer_fifo_u16 *b_u16t, 
	uint16_t length )
{
	cm_disable_interrupts();
	uint16_t j;
	volatile uint16_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) {
		
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
			cm_enable_interrupts();
			// Nothing left, return number of elements retrieved.
			return j;
		}
	}
	cm_enable_interrupts();
	return j;
}