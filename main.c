#include <gb/gb.h>
#include <stdio.h>
#include "gfx/tiles.c"
#include "maps/worldmap.c"
#include "gfx/player.c"

#define MAP_WIDTH 32
#define MAP_HEIGHT 32
#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 18
#define MIN_X 0
#define MIN_Y 0
#define MAX_X (MAP_WIDTH-SCREEN_WIDTH)
#define MAX_Y (MAP_HEIGHT-SCREEN_HEIGHT)
#define PLAYER_SPRITE_IDX 0
#define SPRITE_SIZE 8
#define DIR_DOWN 0
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define ANIMATION_DELAY 8
#define PLAYER_DOWN_START 0
#define PLAYER_UP_START 2
#define PLAYER_LEFT_START 4
#define PLAYER_RIGHT_START 6
#define INPUT_DELAY 6

unsigned char player_x=16;
unsigned char player_y=16;
unsigned char camera_x=6;
unsigned char camera_y=7;
unsigned char input_counter=0;
unsigned char player_dir=DIR_DOWN;
unsigned char player_animation_frame=0;
unsigned char animation_counter=0;
unsigned char player_moving=0;

void init_background(){
    set_bkg_data(0,8,tiles);
    set_bkg_tiles(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,&worldmap[camera_y*MAP_WIDTH+camera_x]);
}

void init_player(){
    set_sprite_data(0,8,playerSprites);
    set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_DOWN_START);
    move_sprite(PLAYER_SPRITE_IDX,80,80);
}

void update_camera(){
    set_bkg_tiles(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,&worldmap[camera_y*MAP_WIDTH+camera_x]);
}

unsigned char can_move_to(unsigned char x,unsigned char y){
    return worldmap[y*MAP_WIDTH+x]<4;
}

void animate_player(){
    if(player_moving){
        animation_counter++;
        if(animation_counter>=ANIMATION_DELAY){
            animation_counter=0;
            player_animation_frame=!player_animation_frame;
            switch(player_dir){
                case DIR_DOWN:
                    set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_DOWN_START+player_animation_frame);
                    break;
                case DIR_UP:
                    set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_UP_START+player_animation_frame);
                    break;
                case DIR_LEFT:
                    set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_LEFT_START+player_animation_frame);
                    break;
                case DIR_RIGHT:
                    set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_RIGHT_START+player_animation_frame);
                    break;
            }
        }
    }else{
        switch(player_dir){
            case DIR_DOWN:
                set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_DOWN_START);
                break;
            case DIR_UP:
                set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_UP_START);
                break;
            case DIR_LEFT:
                set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_LEFT_START);
                break;
            case DIR_RIGHT:
                set_sprite_tile(PLAYER_SPRITE_IDX,PLAYER_RIGHT_START);
                break;
        }
    }
}

void move_player(unsigned char direction){
    unsigned char prev_x=player_x;
    unsigned char prev_y=player_y;
    
    player_dir=direction;
    
    switch(direction){
        case DIR_UP:
            if(player_y>MIN_Y&&can_move_to(player_x,player_y-1))player_y--;
            break;
        case DIR_DOWN:
            if(player_y<MAP_HEIGHT-1&&can_move_to(player_x,player_y+1))player_y++;
            break;
        case DIR_LEFT:
            if(player_x>MIN_X&&can_move_to(player_x-1,player_y))player_x--;
            break;
        case DIR_RIGHT:
            if(player_x<MAP_WIDTH-1&&can_move_to(player_x+1,player_y))player_x++;
            break;
    }
    
    if(prev_x!=player_x||prev_y!=player_y){
        if((signed char)(player_x-camera_x)<5&&camera_x>0){
            camera_x--;
            update_camera();
        }else if((signed char)(player_x-camera_x)>SCREEN_WIDTH-6&&camera_x<MAX_X){
            camera_x++;
            update_camera();
        }
        
        if((signed char)(player_y-camera_y)<5&&camera_y>0){
            camera_y--;
            update_camera();
        }else if((signed char)(player_y-camera_y)>SCREEN_HEIGHT-6&&camera_y<MAX_Y){
            camera_y++;
            update_camera();
        }
    }
    
    move_sprite(PLAYER_SPRITE_IDX,(player_x-camera_x)*SPRITE_SIZE+8,(player_y-camera_y)*SPRITE_SIZE+16);
}

void handle_input(){
    unsigned char joy = joypad();
    
    if(joy&(J_UP|J_DOWN|J_LEFT|J_RIGHT)){
        input_counter++;
        if(input_counter>=INPUT_DELAY){
            input_counter=0;
            player_moving=1;
            
            if(joy&J_UP){
                move_player(DIR_UP);
            }else if(joy&J_DOWN){
                move_player(DIR_DOWN);
            }else if(joy&J_LEFT){
                move_player(DIR_LEFT);
            }else if(joy&J_RIGHT){
                move_player(DIR_RIGHT);
            }
        }
    }else{
        player_moving=0;
        input_counter=INPUT_DELAY; // Ready for next press
    }
}

void show_title_screen(){
    printf("\n\n     GB RPG\n\n");
    printf("   DEMO v1.0\n\n\n");
    printf(" USE D-PAD TO MOVE\n\n");
    printf(" PRESS START BUTTON");
    
    waitpad(J_START);
    
    for(unsigned char i=0;i<20;i++)printf("\n");
}

void main(){
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    
    show_title_screen();
    init_background();
    init_player();
    
    while(1){
        wait_vbl_done();
        handle_input();
        animate_player();
        delay(10); // Faster game loop for smoother movement
    }
}
