#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include<windows.h>
#include<string>
#include <vector>
#include<iostream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"



//Screen
const int FRAME_PER_SECOND = 75; //FPS 
const int MFPS = 50;
const int TIME = 360;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

//Màu để xóa nền Ở ĐÂY
const int COLOR_KEY_R = 167;        
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;
const int BULLET_VAL = 20;

const int RENDER_DRAW_COLOR = 0xff;

#define BLANK_TILE 0
#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
//Trong map01.dat là giá trị của ô chưa tiền
#define STATE_MONEY 24
#define STATE_DIAMOND 25
#define STATE_LIFE 26
#define STATE_GAI1 19
#define STATE_GAI2 20

//Trạng thái của nhân vật ->MainObject
struct Input{
    int left_;
    int right_;
    int up_;
    int down_;
    int stand_;
    int jump_;
};

//Vị trí của các 
//1 biến kiểu Map là 1 ô để fill đất hoặc gạch hoặc chỗ trống
struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_; 

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

namespace SDLCommonFunc{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}
#endif  