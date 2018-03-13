#ifndef BUFFER_H_

#include <stdint.h>
#include <stdio.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>

#define B_SIZE_FIFO_U8T 512
#define B_SIZE_FIFO_U16T 2048

struct buffer_fifo_u8
{
	uint8_t data[B_SIZE_FIFO_U8T];
	uint8_t *getPt; 
	uint8_t *putPt;
	void (*handler_function)( volatile void *data, uint8_t length );
	 
};

struct buffer_fifo_u16
{
	uint16_t data[B_SIZE_FIFO_U16T];
	uint16_t *getPt; 
	uint16_t *putPt;
	void (*handler_function)( volatile void *data, uint8_t length );
	 
};

/* Defines the available queue format (FIFO, circular, etc.) and data type.
*/
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
		struct buffer_fifo_u8 *fifo_u8;
		struct buffer_fifo_u16 *fifo_u16;
	} is;

	int32_t *flagSize;
};

typedef struct buffer_param buffer_param_t;


// ******* Buffer_paramInit *******
// Initializes a buffer parameter structure, preparing it for usage (TO-DO).
// Inputs: A buffer_format enum.
// Ouputs: A buffer_param_t pointer.
buffer_param_t *Buffer_paramInit( enum buffer_format b_format );

// ******* Buffer_init *******
// Initializes a FIFO queue structure, preparing it for usage.
// Inputs: Pointer to buffer_fifo_t.
//		   Function pointer called by the event scheduler to handle queue processing.
// Ouputs: None
void Buffer_init( buffer_param_t *b_param, 
	void(*handler)( volatile void *data, uint8_t length ) );

// ******* Buffer_put *******
// Public function that appends data to a specified buffer based on the
// buffer parameter structure provided.
//  Inputs: pointer to data, pointer to a buffer_param_t, data length
// Outputs: number of elements inserted successfully
uint16_t Buffer_put( volatile void *in_buf, 
	buffer_param_t *b_param, 
	uint16_t length );

// ******* Buffer_get *******
// Public function that retrieves and removes data from a buffer corresponding
// to the supplied buffer parameter structure.
//  Inputs: data pointer, buffer_param_t pointer, and number of elements to
//          read.
// Outputs: number of elements read into the data pointer.
uint16_t Buffer_get( volatile void *out_buf, 
	buffer_param_t *b_param, 
	uint16_t length );

// ******* buffer_fifo_u8_put *******
// Private function that appends data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of elements inserted successfully
uint16_t buffer_fifo_u8_put( volatile void *in_buf, 
	struct buffer_fifo_u8 *b_u8t, 
	uint16_t length );

// ******* buffer_fifo_u8_get *******
// Private function to retrieve and remove data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
uint16_t buffer_fifo_u8_get( volatile void *out_buf, 
	struct buffer_fifo_u8 *b_u8t, 
	uint16_t length );

// ******* buffer_fifo_u16_put *******
// Private function that appends data to a supplied buffer.
//  Inputs: pointer to data, pointer to a buffer_fifo_t, data length
// Outputs: number of elements inserted successfully
uint16_t buffer_fifo_u16_put( volatile void *in_buf, 
	struct buffer_fifo_u16 *b_u16t, 
	uint16_t length );

// ******* buffer_fifo_u16_get *******
// Private function to retrieve and remove data from a specified buffer.
//  Inputs: data pointer, buffer_fifo_t pointer, and number of elements to read
// Outputs: number of elements read into the data pointer.
uint16_t buffer_fifo_u16_get( volatile void *out_buf, 
	struct buffer_fifo_u16 *b_u16t, 
	uint16_t length );

// ******* Buffer_flagSizeAdd *******
// Decrement semaphore, blocking task if less than zero
//  Inputs: pointer to a flag, number of elements to add
// Outputs: none
void Buffer_flagSizeAdd( int32_t *flagPt, uint16_t num_elements );

// ******* Buffer_flagSizeSub *******
// Subtract  buffer size flag
//  Inputs: pointer to a flag, number of elements to substract
// Outputs: none
void Buffer_flagSizeSub( int32_t *flagPt, uint16_t num_elements );

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
extern void Uart_send( volatile void* data, uint16_t length );



#define BUFFER_H_ 1
#endif