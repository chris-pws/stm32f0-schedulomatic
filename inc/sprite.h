#ifndef SPRITE_H_

#include <stdio.h>

struct sprite_obj
{
	uint16_t width;
	uint16_t height;

	static const uint8_t *bmp_data;
	uint16_t length;

}

typedef struct sprite_obj sprite_obj_t;

#define SPRITE_H_ 1
#endif