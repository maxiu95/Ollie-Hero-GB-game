#include <gb/gb.h>
#include <stdio.h>
#include "gamesprites.c"
#include "SkateboardSprites.c"
#include "splashscreen_data.c"
#include "splashscreen_map.c"
#include "tutorialsplash_data.c"
#include "tutorialsplash_map.c"
#include "background_data.c"
#include "background_map.c"


GameSprite skateboard;
GameSprite rock;
GameSprite bird;
UBYTE spritesize = 8;
INT16 playerlocation[2];


// sets the speed of the game
void performantdelay(UINT8 numloops)
{
    UINT8 i;
    for(i = 0; i < numloops; i++)
    {
        wait_vbl_done();
    }
}

// check for sprite collision
UBYTE checkcollisions(GameSprite* one, GameSprite* two)
{
    return (one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

// sets the movement of metasprites
void movegamesprite(struct GameSprite* sprite, UINT8 x, UINT8 y)
{
    move_sprite(sprite->spritids[0], x, y);
    move_sprite(sprite->spritids[1], x + spritesize, y);
}

// setup function for skateboard sprite
void setupskateboard()
{
    skateboard.x = 20;
    skateboard.y = 115;
    skateboard.width = 16;
    skateboard.height = 8;

    // load sprites for skateboard
    set_sprite_tile(0,0);
    skateboard.spritids[0] = 0;
    set_sprite_tile(1,1);
    skateboard.spritids[1] = 1;

    movegamesprite(&skateboard, skateboard.x, skateboard.y);
}

// setup function for the rock sprite 
void setuprock()
{
    rock.x = 100;
    rock.y = 115;
    rock.width = 16;
    rock.height = 8;

    // load sprites for rock obstacle 
    set_sprite_tile(2,2);
    rock.spritids[0] = 2;
    set_sprite_tile(3,3);
    rock.spritids[1] = 3;

    movegamesprite(&rock, rock.x, rock.y);
}

// setup function for bird sprite
void setupbird()
{
    bird.x = 150;
    bird.y = 100;
    bird.width = 16;
    bird.height = 8;

    // load sprites for bird obstacle
    set_sprite_tile(4,4);
    bird.spritids[0] = 4;
    set_sprite_tile(5,5);
    bird.spritids[1] = 5;

    movegamesprite(&bird, bird.x, bird.y);
}

// setup game controls 
void movement()
{
    if(joypad() & J_LEFT)
        {
            if (skateboard.y == 115) // check if skateboard is on ground
            {
                skateboard.x -= 8;
                movegamesprite(&skateboard, skateboard.x, skateboard.y);
            }
        }
        if(joypad() & J_RIGHT)
        {
            if (skateboard.y == 115) // check if skateboard is on ground
            {
                skateboard.x += 8;
                movegamesprite(&skateboard, skateboard.x, skateboard.y);
            }
        }
        if ((joypad() & J_A))
        {
            // sound settings
            NR10_REG = 0x79;
            NR11_REG = 0xC0;
            NR12_REG = 0x42;
            NR13_REG = 0x4A;
            NR14_REG = 0x86;

            if (skateboard.y == 115) // check if skateboard is on ground
            {
                // skateboard jumps up
                skateboard.y -= 15;
                movegamesprite(&skateboard, skateboard.x, skateboard.y);
                //skateboard advances forwards
                skateboard.x += 8;
                movegamesprite(&skateboard, skateboard.x, skateboard.y);
            }
        }
        if ((joypad() & J_B))
        {
            // sound settings
            NR10_REG = 0x79;
            NR11_REG = 0xC0;
            NR12_REG = 0x42;
            NR13_REG = 0x4A;
            NR14_REG = 0x86;

            if (skateboard.y == 100)
            {
                skateboard.x += 8;
                movegamesprite(&skateboard, skateboard.x, skateboard.y);
                // skateboard 
                skateboard.y += 15;
                movegamesprite(&skateboard, skateboard.x, skateboard.y);
            }
        }
}

// set the two splashscreens (title and controls)
void splashscreen_setup()
{
    // game start splashscreen
    set_bkg_data(0, 168, splashscreen_data);
    set_bkg_tiles(0, 0, 20, 18, splashscreen_map);
    // press start to begin
    waitpad(J_START);
    // sound settings
    NR10_REG = 0x79;
    NR11_REG = 0xC0;
    NR12_REG = 0x42;
    NR13_REG = 0x4A;
    NR14_REG = 0x86;
    // game tutorial splashscreen
    set_bkg_data(0, 201, tutorialsplash_data);
    set_bkg_tiles(0, 0, 20, 18, tutorialsplash_map);
    //press start to begin
    waitpad(J_START);
    // sound settings
    NR10_REG = 0x79;
    NR11_REG = 0xC0;
    NR12_REG = 0x42;
    NR13_REG = 0x4A;
    NR14_REG = 0x86;
}

// setup the main background and sprites
void main_setup()
{
    // game background 
    set_bkg_data(0, 4, backgroundsprites);
    set_bkg_tiles(0, 0, 20, 18, background_map);
    set_sprite_data(0, 5, SkateboardSprites);

    // setup sprites
    setupskateboard();
    setuprock();
    setupbird();
}

void main()
{
    // game setup
    SHOW_SPRITES;   
    SHOW_BKG;
    DISPLAY_ON;

    // sound registers to enable sound
    NR52_REG = 0x80; // turns on sound
    NR50_REG = 0x77; // sets volume for both R & L channels (at max)
    NR51_REG = 0xFF; // select channels we want to use- this selects all four

    splashscreen_setup();
    main_setup();
   
    // main game loop
    // before any movement takes place, need to check if there is a collision with the other sprite
    while(!checkcollisions(&skateboard, &rock) && !checkcollisions(&skateboard, &bird))
    {
        movement();
    
        // rock movement across screen
        rock.x -= 10;
        movegamesprite(&rock, rock.x, rock.y);

        // bird movement across screen
        bird.x -= 6;
        movegamesprite(&bird, bird.x, bird.y);

        performantdelay(5);

    }
    printf("\n \n \n \n \n \n \n === GAME  OVER ===");
}

