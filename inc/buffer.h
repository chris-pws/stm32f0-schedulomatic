#ifndef BUFFER_H_

#include <stdint.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>

#define BUFFERSIZE 1024


/*

TO-DO: Define root buffer class, subclassing for different queue types

typedef enum { buffer_fifo_t = 0, buffer_circ_t } buffer_type_e;

typedef struct {
	uint32_t data[BUFFERSIZE+1]; // number of elements in buffer
	uint32_t *getPt; 
	uint32_t *putPt;
	void (*getFunction)( uint32_t *data, uint32_t *getPt, uint32_t *putPt );
	void (*putFunction)( uint32_t *data, uint32_t *getPt, uint32_t *putPt );
	void (*handler_function)( volatile void *data, uint32_t length );

} buffer_t;
*/
typedef struct {
	uint8_t data[BUFFERSIZE+1]; // number of elements in buffer
	uint32_t *getPt; 
	uint32_t *putPt;
	void (*handler_function)( volatile void *data, uint8_t length );

} buffer_fifo_t;

// ******* Buffer_fifoInit *******
// Initializes a FIFO queue structure, preparing it for usage.
// Inputs: Pointer to buffer_fifo_t.
//		   Function pointer called by the event scheduler to handle queue processing.
// Ouputs: None
void Buffer_fifoInit( buffer_fifo_t *buffer, void(*handler)( volatile void *data, uint8_t length ) );

// ******* Buffer_fifoPut *******
// Appends a length of data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of bytes inserted successfully
uint32_t Buffer_fifoPut( volatile void *data, buffer_fifo_t *buffer, uint32_t length );

// ******* Buffer_fifoGet *******
// Retrieves and removes data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of bytes to the read.
// Outputs: number of bytes read into the data pointer.
uint32_t Buffer_fifoGet( volatile void *data, buffer_fifo_t *buffer, uint32_t length );

// ******* Buffer_fifoStat *******
// Retrieves number of items currently stored in buffer.
//  Inputs: buffer_fifo_t pointer
// Outputs: number of bytes contained in the buffer.
uint32_t Buffer_fifoStat( buffer_fifo_t *buffer );

// ******* Buffer_fifoTxEvent *******
// Periodic event that manages transmit queue.
// Executed from the 
//  Inputs: buffer_fifo_t pointer, signal flag
// Outputs: none
void Buffer_fifoTxEvent( buffer_fifo_t *buffer, int32_t *flagPt );

// ******* Sched_flagWait *******
// Decrement semaphore, blocking task if less than zero
// Inputs: pointer to a counting semaphore
extern void Sched_flagWait( int32_t *semaPt );

#define BUFFER_H_ 1
#endif