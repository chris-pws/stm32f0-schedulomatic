#ifndef FRAME_H_

#include <stdio.h>

#define bit_mask(bit_offset, bit_count) 					\
(((1 << bit_count) - 1) << bit_offset)

#define get_bits(target, bit_offset, bit_count)  			\
((target & bit_mask(bit_offset, bit_count)) >> bit_offset)

#define write_bits(target, bit_offset, bit_count, value)  	\
{target = (target & ~bit_mask(bit_offset, bit_count)) | (value << bit_offset);}

struct frame_buffer
{
	volatile uint8_t *data;
	int width;
	int h_width;
	int length;
	volatile int32_t *readyFlag;
};

typedef struct frame_buffer frame_buffer_t;

/******** frame_bufferInit *********
* Initializes a frame_buffer_t object.
*  Inputs: pointer to a frame_buffer_t, width, length, pointer to an allocated
*  data array.
* Outputs: none
*/
void frame_bufferInit( frame_buffer_t *f, int width, int length, 
					volatile uint8_t *data, volatile int32_t *flag );

/******** frame_pixelSet *********
* Sets an arbitary pixel to greyLvl reflecting the x,y coordinates provided.
*  Inputs: pointer to a frame_buffer_t, x coordinate, y coordinate, grey level
*  from 0 (black) to F (white).
* Outputs: none
*
* frame_buffer_t:
*	member h_width is expected to be ceiling half the width
*/
void frame_pixelSet( frame_buffer_t *f, int x, int y, int value );

/******** frame_pixelGet *********
* Returns a pixel's greyLvl located by the x,y coordinates provided.
*  Inputs: pointer to a frame_buffer_t, x coordinate, y coordinate.
* Outputs: grey level from 0 (black) to F (white).
*/
uint8_t frame_pixelGet( frame_buffer_t *f, int x, int y );

#define FRAME_H_ 1
#endif