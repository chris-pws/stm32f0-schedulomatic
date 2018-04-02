#ifndef FRAME_H_

#include <stdio.h>

struct frame_buffer
{
	volatile uint8_t *data;
	uint16_t width;
	uint16_t length;
	int32_t *readyFlag;
};

typedef struct frame_buffer frame_buffer_t;

void frame_bufferInit( frame_buffer_t *f, uint16_t width, uint16_t length, 
					volatile uint8_t *data );

void frame_pixelSet( frame_buffer_t *f, uint16_t x, uint16_t y, 
					uint8_t greyLvl );

uint8_t frame_pixelGet( frame_buffer_t *f, uint16_t x, uint16_t y );

#define FRAME_H_ 1
#endif