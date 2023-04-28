#include"BulletObject.h"

BulletObject::BulletObject(){
    x_val_ = 0;
    y_val_ = 0;

    height_frame_ = GetRect().x/8;
    width_frame_  = GetRect().y;
    //Để ban đầu là vẫn trong map
    is_move_ = false;
    bullet_type_ = SPHERE_BULLET;
}

BulletObject::~BulletObject(){
}

bool BulletObject::LoadImgBullet(SDL_Renderer* des){
    bool ret = false;
    if( bullet_type_ == LASER_BULLET){
        ret = LoadImg("assets/img/map/laser_bullet.png",des);
    }
    else if (bullet_type_ == SPHERE_BULLET){
        ret = LoadImg("assets/img/map/sphere_bullet.png",des);
    }
    return ret;
}

void BulletObject::HandleMove(const int& x_border, const int& y_border){
    if(bullet_dir_ == DIR_RIGHT)
    {//Vị trí của 1 đối tượng hình ảnh hiển thị trên màn hình
        x_val_ = BULLET_VAL;
        rect_.x += x_val_;
        if(rect_.x > x_border){
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_LEFT){ 
        x_val_ = (-1)*BULLET_VAL;
        rect_.x += x_val_ ;
        if(rect_.x < 0){
            is_move_ = false;
        }
    }
}


void BulletObject:: CheckToMap(Map &map_data){
    //Định vị trí đầu cuối
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;
    int x_pos_ = GetRect().x;
    int y_pos_ = GetRect().y;

                        //KIỂM TRA THEO CHIỀU NGANG
    //Nếu nhỏ hơn TILE_SIZE thì lấy height_frame còn không thì lấy TILE_SIZE
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    //Xác định Ô BẮT ĐẦU mà nhân vật đang đứng
    x1 = (x_pos_ + x_val_) /TILE_SIZE;
    //Xác định Ô KẾT THÚC mà nhân vật đứng
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;  

    
    //Xác định ô đầu cuối mà nhân vật đang đứng 
    y1 = (y_pos_) /TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

    /*
        x1,y1********x2,y1
        *               *
        *               *
        *               *
        x1,y2********x2,y2
    */

    //Kiểm tra xem có đang nằm trong bản đồ không
    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
        if(x_val_ > 0) //Di chuyển sang phải
        {   
            //Nếu ăn phải tiền thì ô tiền biến mất
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];
            //Kiểm tra xem ô bên phải có phải là ô trống hay không
            if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18)){
                is_move_ = false;
            }
            
        }
        //di chuyển sang trái
        else if (x_val_ < 0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            //Nếu ăn phải tiền thì ô tiền biến mất
            if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18)){
                is_move_ = false;
            }
        }   
    }    

                    //KIỂM TRA THEO CHIỀU DỌC
    int width_min = width_frame_ <TILE_SIZE ? width_frame_ :TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min) /TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ -1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
        //Rơi tự do
        if(y_val_ > 0){
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];

            if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18)){
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= (height_frame_ +1);
                y_val_ = 0;
            }
            
        }
         else if( y_val_ <0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];

            if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18) ){
                    y_pos_ = (y1 + 1)*TILE_SIZE;
                y_val_ = 0;
            }

        }
    }
    x_pos_ += x_val_;
    y_pos_ += y_val_;
    //Lùi quá mép bản đồ
    if(x_pos_ < 0){
        //Cho nó dừng lại luôn
        x_pos_ = 0;
    }
    else if(x_pos_ + width_frame_ > map_data.max_x_){
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }

}
