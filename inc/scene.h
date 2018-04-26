#ifndef SCENE_H_

#include <stdio.h>

enum scene_tiling
{
	NONE = 0,
	TILE_X,
	TILE_Y,
	TILE_XY
};

struct scene_backgnd
{
	static const sprite_obj_t;
	uint16_t x;
	uint16_t y;
	enum scene_tiling;
};

struct scene_obj
{
	uint16_t x;
	uint16_t y;
	sprite_obj_t *sprite;
	scene_obj_t *next;
	//additional attributes
	
}

#define SCENE_H_ 1
#endif