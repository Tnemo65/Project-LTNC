#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include <vector>

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "PlayerPower.h"
#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 9
#define PLAYER_JUMP_VAL 15
using namespace std;
class MainObject : public BaseObject
{
    public:
        MainObject();
        ~MainObject();

        //Con số ứng với trạng thái trái hay phải
        enum WalkType{
            WALK_NONE = 0,
            WALK_RIGHT = 1,
            WALK_LEFT = 2,
            STAND = 3,
        };

        bool LoadImg (string path, SDL_Renderer* screen);
        //Hiện ảnh theo frame
        void Show(SDL_Renderer* screen);
        //Hàm xử lí khi bấm trái phải
        void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk*bullet_sound[2]);
        void set_clips();
        //Cần truyền vào bản đồ để biết giới hạn đầu cuối để không cho đi qua
        void DoPlayer(Map& map_data);
        void CheckToMap(Map& map_data);
        //map_x, map_y là lưu vị trí bản đồ
        void SetMapXY(const int map_x, const int map_y){map_x_ = map_x; map_y_ = map_y;}
        //Lấy vị trí trung tâm map
        void CenterEntityOnMap(Map& map_data);
        //Update hình ảnh nhân vật
        void UpdateImagePlayer(SDL_Renderer* des);
        //Cập nhật 1 vector chứa các viên đạn
        SDL_Rect GetRectFrame();
        void set_bullet_list(vector<BulletObject*> bullet_list){
            p_bullet_list_ = bullet_list;
        }
        vector<BulletObject*> get_bullet_list() const {return  p_bullet_list_;}
        void HandleBullet(SDL_Renderer* des);
        void RemoveBullet(const int& idx);
        void IncreaseMoney();

        int get_frame_width() const {return width_frame_;}
        int get_frame_height() const {return height_frame_;}
        void set_comeback_time(const int& cb_time) {come_back_time_ = cb_time;}
        int get_comeback_time() {return come_back_time_;}

        //LayTien
        int GetMoneyCount() const {return money_count;}


        int lifeNumber = 0;
        bool IsLifeIncrease(){ return IsIncreaseLife;}
        void SetLifeIncrease(bool status){ IsIncreaseLife = status;}
        bool IsLifeDecrease(){ return IsDecreaseLife;}
        void SetLifeDecrease(bool status){ IsDecreaseLife = status;}


        void SetMarkCount (int markcount) {mark = markcount;}
        int GetMarkCount() const{return mark;}
        void SetLifeCount(int lifecount) {life = lifecount;}
        int GetLifeCount() const{return life;}
        void SetTimeCount(int timecount){time = timecount;}
        int GetTimeCount() const {return time;}

        
        bool IsWin(){return win;}
        void SetWin(bool status){ win = status;}
    private:
        //Vector chứa những viên đạn
        vector<BulletObject*> p_bullet_list_;
        //Khi bấm trái phải đi 1 lượng bao nhiêu
        float x_val_;
        float y_val_;
        
        //Vị trí hiện tại của nhân vật để lên ảnh
        float x_pos_;
        float y_pos_;

        //Vì ảnh là chiều dài của 8 frame
        //Cần lấy kích thước của nhân vật trong 1 frame vì mình chỉ cần 1 để hiện ra
        int width_frame_;
        int height_frame_;

        //SỐ FRAME Ở ĐÂY -> THAY ĐỔI = 16 FRAME Ở ĐÂY
        SDL_Rect frame_clip_[8];
        //Biến lưu trạng thái
        Input input_type_;
        //Biến lưu chỉ số frame
        int frame_;
        //Biến lưu trạng thái phải hay trái
        int status_;
        bool on_ground_;
        //Vị trí của map để làm map di chuyển 
        int map_x_;
        int map_y_;
        bool double_jump_;
        int come_back_time_;
        bool IsIncreaseLife;
        bool IsDecreaseLife;
        bool win;

        int money_count;
        int mark;
        int life;
        int time;
};

#endif