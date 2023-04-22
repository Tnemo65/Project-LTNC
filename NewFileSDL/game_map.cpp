#include "game_map.h"


void GameMap :: LoadMap(char* name)
{
    FILE* fp = fopen(name, "rb");
    //fopen_s(&fp, name, "rb");
    if(fp == NULL){
        return;
    }
    game_map_.max_x_ = 0;
    game_map_.max_y_ = 0;
    for(int i = 0; i < MAX_MAP_Y; i++)
    {
        for(int j = 0; j < MAX_MAP_X; j++)
        {
            //Đọc một số nguyên từ tệp được mở fp và lưu trữ giá trị đó vào biến game_map_.tile[i][j]
            fscanf(fp, "%d", &game_map_.tile[i][j]);
            int val = game_map_.tile[i][j];
            //Nếu là 1 2 3 là các loại đá gạch
            if(val >0){
                if( j > game_map_.max_x_){
                    game_map_.max_x_ = j;
                }

                if(i > game_map_.max_y_){
                    game_map_.max_y_ = i;
                }
            }
        }
    }
    game_map_.max_x_ = (game_map_.max_x_ + 1) *TILE_SIZE;
    game_map_.max_y_ = (game_map_.max_y_ + 1) *TILE_SIZE;
    
    game_map_.start_x_ = 0;
    game_map_.start_y_ = 0;
    game_map_.file_name_ = name;
    fclose(fp);
}

void GameMap:: LoadTiles (SDL_Renderer* screen){
    char file_img[30];
    FILE* fp = NULL;
    for(int i = 0; i < MAX_TILES; i++){
        //Định dạng chuỗi kí tự "" lưu vào sprintf 
        //Tạo ra tên file ảnh cho file_img
        sprintf(file_img, "assets/img/map/%d.png", i);
        //Mở tệp hình ảnh với tên file là file_img
        fp = fopen( file_img, "rb");
        if(fp == NULL){
            continue;
        }
        fclose(fp);
        tile_mat[i].LoadImg(file_img, screen);
    }
}

void GameMap::DrawMap(SDL_Renderer* screen){
    //x1 đầu màn, x2 cuối màn
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;
    int map_x = 0;
    int map_y = 0;
    map_x = game_map_.start_x_ / TILE_SIZE;
    //TILE_SIZE là độ lớn cạnh của ô vuông
    //x1 sẽ là tọa độ(đơn vị pixel) của vị trí bản đồ sau khi đưa về ô vuông gần nhất
    //Ô vuông gần nhất sẽ về bên trái để không fill khuyết phần hổng nếu như x đang ở giữa ô
    x1 =(game_map_.start_x_ %TILE_SIZE) * (-1);
    //x2 cuối map sẽ fill quá thêm một lượng = với lượng hổng của x1(nếu x1 !=0)
    //Nếu x1=0 thì + 0 còn nếu khác thì + Tile_size
    x2 =x1 + SCREEN_WIDTH + (x1 ? TILE_SIZE : 0);

    map_y = game_map_.start_y_/TILE_SIZE;
    y1 = (game_map_.start_y_% TILE_SIZE) * (-1);
    y2 = y1+ SCREEN_HEIGHT +(y1 ?  TILE_SIZE : 0);

    for(int i = y1; i < y2; i+= TILE_SIZE){
        map_x = game_map_.start_x_/ TILE_SIZE;
        for(int j = x1; j < x2; j += TILE_SIZE){
            int val = game_map_.tile[map_y][map_x];
            if(val > 0){
                tile_mat[val].SetRect(j, i);
                tile_mat[val].Render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}