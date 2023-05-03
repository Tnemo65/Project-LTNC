#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunc.h"
class BulletObject: public BaseObject
{
    public:
        BulletObject();
        virtual ~BulletObject();
        enum BulletDir{
            DIR_RIGHT = 20,
            DIR_LEFT = 21,
        };
        
        enum BulletType{
            SPHERE_BULLET = 50,
            LASER_BULLET = 51,
            LASER_BULLET1 = 52,
        };

        void set_x_val(const int& xVal) {x_val_ = xVal;}
        void set_y_val(const int& yVal) {y_val_ = yVal;}
        int get_x_val() const {return x_val_;}
        int get_y_val() const {return y_val_;}

        void set_is_move(const bool& isMove) {is_move_ = isMove;}
        bool get_is_move() const {return is_move_;}

        void set_bullet_dir(const unsigned int& bulletDir){
            bullet_dir_ =bulletDir;
        }
        unsigned get_bullet_dir() const {return bullet_dir_;}
        
        void set_bullet_type(const unsigned int& bulletType) {bullet_type_ = bulletType;}
        unsigned int get_bullet_type() const {return bullet_type_;}        
        
        //Để xác định biên x y, nếu vượt quá màn hình thì xóa nó đi
        void HandleMove(const int& x_border, const int& y_border);
        bool LoadImgBullet(SDL_Renderer* des);

        void CheckToMap(Map& map_data);
    private:
        int x_val_;
        int y_val_;
        //Trong màn hình thì chạy không thì xóa
        bool is_move_;
        //Xác định vị trí viên đạn được bắn ra theo HƯỚNG CỦA NHÂN VẬT
        unsigned int bullet_dir_; 
        unsigned int bullet_type_;

        int height_frame_;
        int width_frame_;

};

#endif