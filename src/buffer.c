#include "buffer.h"

// ******* Buffer_fifoInit *******
// Initializes a FIFO queue structure, preparing it for usage.
// Inputs: Pointer to buffer_fifo_t.
//		   Function pointer called by the event scheduler to handle queue processing.
// Ouputs: None
void Buffer_fifoInit( buffer_fifo_t *buffer, void(*handler)( volatile void *data, uint8_t length ) )
{
	buffer->getPt = (uint32_t*)&buffer->data[0];
	buffer->putPt = (uint32_t*)&buffer->data[0];	
	buffer->handler_function = handler;

}

// ******* Buffer_fifoPut *******
// Appends a length of data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of bytes inserted successfully
uint32_t Buffer_fifoPut( volatile void *data, buffer_fifo_t *buffer, uint32_t length )
{
	cm_disable_interrupts();
	uint32_t j;
	uint32_t *nextPutPt;
	nextPutPt = &buffer->putPt[1];
	volatile uint8_t *p;
	p = data;
	for ( j=0; j < length; j++ )
	{
		// testing for end of buffer
		// Check to see if there's space in the buffer
		// full if advancing the putPt to the next element in the buffer is met with the getPt
		// also check for this condition when we reach the buffer length.

		if ( nextPutPt == (uint32_t*)&buffer->data[BUFFERSIZE] ) 
		{
			nextPutPt = (uint32_t*)&buffer->data[0];
		}
		if ( nextPutPt == (uint32_t*)&buffer->getPt )
		{
			cm_enable_interrupts();
			return j; // no room left, return number of chars fetched
		}

		(*buffer->putPt) = *p++;
		buffer->putPt = (uint32_t*)&buffer->putPt[1]; // advance to next buffer byte

		// if we reach the buffer size, then wrap the putPt
		if ( buffer->putPt == (uint32_t*)&buffer->data[BUFFERSIZE] )
			{
				buffer->putPt = (uint32_t*)&buffer->data[0];
			}
		
	}
	cm_enable_interrupts();
	return j; // return number of chars added to the buffer
}

// ******* Buffer_fifoGet *******
// Retrieves and removes data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of bytes to the read.
// Outputs: number of bytes read into the data pointer.
uint32_t Buffer_fifoGet( volatile void *data, buffer_fifo_t *buffer, uint32_t length )
{
	cm_disable_interrupts();
	uint32_t j;
	volatile uint8_t *p;
	//uint32_t *tmpGet;
	//tmpGet = (uint32_t*) buffer->getPt;
	p = data;

	for ( j = 0; j < length; j++ ) {
		
		//if (tmpTail != buffer->head)
		if ( buffer->getPt != buffer->putPt )
		{

			*p++ = (*buffer->getPt);

			buffer->getPt = (uint32_t*)&buffer->getPt[1];

			if ( buffer->getPt == (uint32_t*)&buffer->data[BUFFERSIZE] )
			{
				buffer->getPt = (uint32_t*)&buffer->data[0];
			}

		}
		else
		{
			cm_enable_interrupts();
			
			return j;
		}
	}
	cm_enable_interrupts();
	return j;
}

