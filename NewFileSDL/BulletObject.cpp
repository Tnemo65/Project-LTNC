#include"BulletObject.h"

BulletObject::BulletObject(){
    x_val_ = 0;
    y_val_ = 0;
    //Để ban đầu là vẫn trong map
    is_move_ = false;
}

BulletObject::~BulletObject(){
}

void BulletObject::HandleMove(const int& x_border, const int& y_border){
    if(bullet_dir_ == DIR_RIGHT)
    {//Vị trí của 1 đối tượng hình ảnh hiển thị trên màn hình
        rect_.x += x_val_;
        if(rect_.x > x_border){
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_LEFT){
        
        rect_.x -= x_val_;
        if(rect_.x < 0){
            is_move_ = false;
        }
    }
}