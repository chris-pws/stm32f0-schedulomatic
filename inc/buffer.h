#ifndef BUFFER_H_

#include <stdint.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>

#define B_SIZE_FIFO_U8T 1024
#define B_SIZE_FIFO_U16T 1024

typedef struct buffer_fifo_u8_t
{
	uint8_t data[B_SIZE_FIFO_U8T];
	uint32_t *getPt; 
	uint32_t *putPt;
	void (*handler_function)( volatile void *data, uint8_t length );
	 
} buffer_fifo_u8_t;

typedef struct buffer_fifo_u16_t
{
	uint8_t data[B_SIZE_FIFO_U16T];
	uint32_t *getPt; 
	uint32_t *putPt;
	void (*handler_function)( volatile void *data, uint8_t length );
	 
} buffer_fifo_u16_t;

enum buffer_format
{ 
	FIFO_U8T = 0, 
	FIFO_U16T 
};

struct buffer_param
{
	enum buffer_format type;
	union
	{
		buffer_fifo_u8_t *fifo_u8;
		buffer_fifo_u16_t *fifo_u16;
	} is;
};

typedef struct buffer_param buffer_param_t;


// ******* Buffer_paramInit *******
// Initializes a FIFO format structure, using the buffer format and data type
// associated with the buffer_format_t.
// Inputs: Pointer a buffer_format_t enum
// Ouputs: A populated buffer_param_t
buffer_param_t *Buffer_paramInit( enum buffer_format b_format );

// ******* Buffer_init *******
// Initializes a FIFO queue structure, preparing it for usage.
// Inputs: Pointer to buffer_fifo_t.
//		   Function pointer called by the event scheduler to handle queue processing.
// Ouputs: None
void Buffer_init( buffer_param_t *b_param, 
	void(*handler)( volatile void *data, uint8_t length ) );

// ******* Buffer_put *******
// Appends a length of data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of bytes inserted successfully
uint32_t Buffer_put( volatile void *in_buf, 
	buffer_param_t *b_param, 
	uint32_t length );

// ******* Buffer_get *******
// Retrieves and removes data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of bytes to the read.
// Outputs: number of bytes read into the data pointer.
uint32_t Buffer_get( volatile void *out_buf, 
	buffer_param_t *b_param, 
	uint32_t length );

// ******* buffer_fifo_u8_put *******
// Appends a length of data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of bytes inserted successfully
uint32_t buffer_fifo_u8_put( volatile void *in_buf, 
	buffer_fifo_u8_t *b_u8t, 
	uint32_t length );

// ******* buffer_fifo_u8_get *******
// Retrieves and removes data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of bytes to the read.
// Outputs: number of bytes read into the data pointer.
uint32_t buffer_fifo_u8_get( volatile void *out_buf, 
	buffer_fifo_u8_t *b_u8t, 
	uint32_t length );

// ******* buffer_fifo_u16_put *******
// Appends a length of data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of bytes inserted successfully
uint32_t buffer_fifo_u16_put( volatile void *in_buf, 
	buffer_fifo_u16_t *b_u16t, 
	uint32_t length );

// ******* buffer_fifo_u16_get *******
// Retrieves and removes data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of bytes to the read.
// Outputs: number of bytes read into the data pointer.
uint32_t buffer_fifo_u16_get( volatile void *out_buf, 
	buffer_fifo_u16_t *b_u16t, 
	uint32_t length );

// ******* Buffer_Stat *******
// Retrieves number of items currently stored in buffer.
//  Inputs: buffer_fifo_t pointer
// Outputs: number of bytes contained in the buffer.
uint32_t Buffer_stat( buffer_param_t *buffer );

#define BUFFER_H_ 1
#endif