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
			buffer->is.fifo_u8->getPt = (uint32_t*)&buffer->is.fifo_u8->data[0];
			buffer->is.fifo_u8->putPt = (uint32_t*)&buffer->is.fifo_u8->data[0];
			buffer->is.fifo_u8->handler_function = handler;

		}

		case FIFO_U16T:
		{
			buffer->is.fifo_u16->getPt = (uint32_t*)&buffer->is.fifo_u16->data[0];
			buffer->is.fifo_u16->putPt = (uint32_t*)&buffer->is.fifo_u16->data[0];
			buffer->is.fifo_u16->handler_function = handler;

		}
	}

}

// ******* Buffer_put *******
// Public function that appends data to a specified buffer based on the
// buffer parameter structure provided.
//  Inputs: pointer to data, pointer to a buffer_param_t, data length
// Outputs: number of elements inserted successfully
uint32_t Buffer_put( volatile void *in_buf, 
	buffer_param_t *buffer, 
	uint32_t length )
{
	uint32_t num_queued;

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
uint32_t Buffer_get( volatile void *out_buf, 
	buffer_param_t *buffer, 
	uint32_t length )
{
	uint32_t num_read;

	/*
	*  Call the specialized helper function corresponding to the supplied 
	*  buffer parameters.
	*/
	switch ( buffer->type )
	{
		uint8_t s;
		char test[20];
		case FIFO_U8T:
			num_read = buffer_fifo_u8_get( out_buf, buffer->is.fifo_u8, length );

			return num_read;
			break;

		case FIFO_U16T:

			num_read = buffer_fifo_u16_get( out_buf, buffer->is.fifo_u16, length );
			//s = sprintf(test, "%ld", num_read);
			//Uart_send(test, s);
			return num_read;
			break;

	}
	return 0;
}

// ******* buffer_fifo_u8_put *******
// Private function that appends data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of elements inserted successfully
uint32_t buffer_fifo_u8_put( volatile void *in_buf, 
	struct buffer_fifo_u8 *b_u8t, 
	uint32_t length )
{
	cm_disable_interrupts();
	uint32_t j;
	uint32_t *nextPutPt = &b_u8t->putPt[1];
	volatile uint8_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for buffer overflow
		*  Check to see if there's space in the buffer. Full condition reached
		*  if advancing the putPt to the next element in the buffer is met with
		*  the getPt. First handle wrapping if we've reached the buffersize.
		*/ 
		if ( nextPutPt == (uint32_t*)&b_u8t->data[B_SIZE_FIFO_U8T] ) 
		{
			nextPutPt = (uint32_t*)&b_u8t->data[0];
		}

		if ( nextPutPt == (uint32_t*)&b_u8t->getPt )
		{
			cm_enable_interrupts();
			return j; // no vacancy, return number of elements fetched
		}

		(*b_u8t->putPt) = *p++;

		// advance pointer address to next buffer element
		b_u8t->putPt = (uint32_t*)&b_u8t->putPt[1]; 

		// if we reach the buffer size, then wrap the putPt
		if ( b_u8t->putPt == (uint32_t*)&b_u8t->data[B_SIZE_FIFO_U8T] )
			{
				b_u8t->putPt = (uint32_t*)&b_u8t->data[0];
			}
		
	}
	cm_enable_interrupts();
	return j; // return number of data added to the buffer
}

// ******* buffer_fifo_u8_get *******
// Private function to retrieve and remove data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
uint32_t buffer_fifo_u8_get( volatile void *out_buf, 
	struct buffer_fifo_u8 *b_u8t, 
	uint32_t length )
{
	cm_disable_interrupts();
	uint32_t j;
	volatile uint8_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) {
		
		// Check for get & put pointer collision
		if ( b_u8t->getPt != b_u8t->putPt )
		{
			*p++ = (*b_u8t->getPt);

			b_u8t->getPt = (uint32_t*)&b_u8t->getPt[1];

			// Check for pointer wrap-around
			if ( b_u8t->getPt == (uint32_t*)&b_u8t->data[B_SIZE_FIFO_U8T] )
			{
				b_u8t->getPt = (uint32_t*)&b_u8t->data[0];
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
uint32_t buffer_fifo_u16_put( volatile void *in_buf, 
	struct buffer_fifo_u16 *b_u16t, 
	uint32_t length )
{
	cm_disable_interrupts();
	uint32_t j;
	uint32_t *nextPutPt = (uint32_t*)&b_u16t->putPt;

	volatile uint16_t *p;

	p = in_buf;

	for ( j = 0; j < length; j++ )
	{
		/* Testing for buffer overflow
		*  Check to see if there's space in the buffer. Full condition reached
		*  if advancing the putPt to the next element in the buffer is met with
		*  the getPt. First handle wrapping if we've reached the buffersize.
		*/ 

		if ( nextPutPt == (uint32_t*)&b_u16t->data[B_SIZE_FIFO_U16T] ) 
		{
			nextPutPt = (uint32_t*)&b_u16t->data[0];
		}

		if ( nextPutPt == (uint32_t*)&b_u16t->getPt )
		{
			cm_enable_interrupts();
			return j; // no vacancy, return number of elements fetched
		}

		(*b_u16t->putPt) = *p++;
		// advance pointer address to next buffer element
		b_u16t->putPt = (uint32_t*)&b_u16t->putPt[1]; 

		// if we reach the buffer size, then wrap the putPt
		if ( b_u16t->putPt == (uint32_t*)&b_u16t->data[B_SIZE_FIFO_U16T] )
			{
				b_u16t->putPt = (uint32_t*)&b_u16t->data[0];
			}
		
	}
	cm_enable_interrupts();
	return j; // return number of data added to the buffer
}

// ******* buffer_fifo_u16_get *******
// Private function to retrieve and remove data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
uint32_t buffer_fifo_u16_get( volatile void *out_buf, 
	struct buffer_fifo_u16 *b_u16t, 
	uint32_t length )
{
	cm_disable_interrupts();
	uint32_t j;
	volatile uint16_t *p;

	p = out_buf;

	for ( j = 0; j < length; j++ ) {
		
		// Check for get & put pointer collision
		if ( b_u16t->getPt != b_u16t->putPt )
		{

			*p++ = (*b_u16t->getPt);

			b_u16t->getPt = (uint32_t*)&b_u16t->getPt[1];

			// Check for pointer wrap-around
			if ( b_u16t->getPt == (uint32_t*)&b_u16t->data[B_SIZE_FIFO_U16T] )
			{
				b_u16t->getPt = (uint32_t*)&b_u16t->data[0];
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