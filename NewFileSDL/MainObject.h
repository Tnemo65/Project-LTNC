#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include <vector>

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 25
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
        };

        bool LoadImg (std::string path, SDL_Renderer* screen);
        //Hiện ảnh theo frame
        void Show(SDL_Renderer* screen);
        //Hàm xử lí khi bấm trái phải
        void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
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
        void set_bullet_list(std::vector<BulletObject*> bullet_list){
            p_bullet_list_ = bullet_list;
        }
        std::vector<BulletObject*> get_bullet_list() const {return  p_bullet_list_;}
        void HandleBullet(SDL_Renderer* des);
        void IncreaseMoney();
    
    private:
        //Vector chứa những viên đạn
        std::vector<BulletObject*> p_bullet_list_;
        int money_count;
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
        
        int come_back_time_;
};

#endif