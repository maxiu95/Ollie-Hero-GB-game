#include <gb/gb.h>

//generical character structure: id, position, graphics
typedef struct GameSprite {
	UBYTE spritids[2]; // all characters use 2 sprites
	UINT8 x;
	UINT8 y;
	UINT8 width;
	UINT8 height;
} GameSprite ;