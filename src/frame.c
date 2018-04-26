#include "frame.h"

/******** frame_bufferInit *********
* Initializes a frame_buffer_t object.
*  Inputs: pointer to a frame_buffer_t, width, length, pointer to an allocated
*  data array.
* Outputs: none
*/
void frame_bufferInit( frame_buffer_t *f, int width, int length, 
					volatile uint8_t *data, volatile int32_t *flag )
{
	f->width = width;
	f->length = length;
	f->data = data;
	f->readyFlag = flag;
}

/******** frame_pixelSet *********
* Sets an arbitary pixel to greyLvl reflecting the x,y coordinates provided.
*  Inputs: pointer to a frame_buffer_t, x coordinate, y coordinate, grey level
*  from 0 (black) to F (white).
* Outputs: none
*
* frame_buffer_t:
*	member h_width is expected to be ceiling half the width
*/
void frame_pixelSet( frame_buffer_t *f, int x, int y, int value ) 
{
	// Bounds check, just make sure to call frame_pixelSet with allowed values for x, y and value. If your value is > 4 bit it will bleed over on other pixels
	// if ((x < 0) || (y < 0) || (x >= f->width) || (y >= f->height)) {
	// 	return;	//Abort update if out of bounds
	// }
	
	int index = y * f->h_width + (x >> 1);
	write_bits(f->data[index], (x & 1) << 2, 4, value);
	//(x & 1) << 2 will be 0 for even and 4 for odd

}

/******** frame_pixelGet *********
* Returns a pixel's greyLvl located by the x,y coordinates provided.
*  Inputs: pointer to a frame_buffer_t, x coordinate, y coordinate.
* Outputs: grey level from 0 (black) to F (white).
*/
uint8_t frame_pixelGet( frame_buffer_t *f, int x, int y )
{
	return 0;
}