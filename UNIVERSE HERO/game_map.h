#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 30
class TileMat : public BaseObject
{
    public:
    TileMat() {;}
    ~TileMat() {;}
};
class GameMap
{
    public:
        GameMap() {;}
        ~GameMap() {;}
    //Đọc hết thông tin trong file text của map
    void LoadMap(char* name);
    //Load ảnh (số 0 load ảnh gì, số 1 load ảnh gì)
    void LoadTiles(SDL_Renderer* screen);
    //Fill ảnh vào vị trí
    void DrawMap(SDL_Renderer* screen);
    Map getMap() const{return game_map_;};
    //Map di chuyển theo nhân vật
    void SetMap(Map& map_data){game_map_ = map_data;};
    private:
        //game_map_ chứa toàn bộ thoong tin bản đồ: vị trí, trạng thái, chỉ số
        Map game_map_;
        //mảng cho các dạng map (gạch, đất, đá)
        TileMat tile_mat[MAX_TILES];
};


#endif