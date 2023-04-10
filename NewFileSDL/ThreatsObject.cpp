#include "ThreatsObject.h"

ThreatsObject :: ThreatsObject(){
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = false;
    come_back_time_ = 0;
    frame_ = 0;

    animation_a_ = 0;
    animation_b_ = 0;
    //Khởi đầu là quay trái
    input_type_.left_ = 0;
    type_move_ = STATIC_THREAT;

}

ThreatsObject::~ThreatsObject(){

}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen){
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret){
        width_frame_ = rect_.w/THREAT_FRAME_NUM;
        height_frame_ = rect_.h;
    }
    return ret;
}

void ThreatsObject::set_clips(){
    if(width_frame_ > 0 && height_frame_ > 0){
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_; 
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2*width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3*width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;

        frame_clip_[4].x = 4*width_frame_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = height_frame_;

        frame_clip_[5].x = 5*width_frame_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = height_frame_;

        frame_clip_[6].x = 6*width_frame_;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = width_frame_;
        frame_clip_[6].h = height_frame_;

        frame_clip_[7].x = 7*width_frame_;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = width_frame_;
        frame_clip_[7].h = height_frame_;
    }
}

void ThreatsObject :: Show(SDL_Renderer* des){
    if(come_back_time_ == 0){
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if(frame_ >= 8){
            frame_ = 0;
        }
        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect rendQuad =  {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_ , currentClip, &rendQuad);
    }
}

 void ThreatsObject ::DoPlayer(Map& gMap){
    if(come_back_time_  == 0){
        x_val_ = 0;
        y_val_ += THREAT_GRAVITY_SPEED;
        if(y_val_ >= THREAT_MAX_FALL_SPEED){
            y_val_ = THREAT_MAX_FALL_SPEED;
        }
        //x_val_ cứ tăng x_pos_ được cộng x_val_ ở cuối hàm CheckToMap cho đến khi ở hàm ImpMoveType thì chặn 2 đầu a b, đến a thì quay phải, đến b thì trái
        if(input_type_.left_ == 1){
            x_val_ -= THREAT_SPEED;
        }
        else if(input_type_.right_ == 1){
            x_val_ += THREAT_SPEED ;
        }
        CheckToMap(gMap);
    }
    else if(come_back_time_ > 0){
        come_back_time_ --;
        if( come_back_time_ == 0){
            InitThreats();  
        }
    }
 }
void ThreatsObject::InitThreats(){
    x_val_ = 0;
    y_val_ = 0;
    if(x_pos_ > 256){
        x_pos_ -= 256;
        //Trừ cả khoảng luôn 
        animation_a_ -= 256;
        animation_b_ -= 256;
    }
    else{
        x_pos_ = 0;
    }
    y_pos_ = 0;
    come_back_time_ = 0;     
    input_type_.left_ = 1;   
}
 void ThreatsObject::CheckToMap(Map& map_data){
    //Định vị trí đầu cuối
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

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
            if( (val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2  != BLANK_TILE && val2 != STATE_MONEY)){
                x_pos_ = x2 * TILE_SIZE; // Ra vị trí biên của frame nhân vật
                //Nếu mà chạm tường thì x_pos_ chỉ dừng ở đó
                //x_val_ = 0
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0;
            } 
        }
        //di chuyển sang trái
        else if (x_val_ < 0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            //Nếu ăn phải tiền thì ô tiền biến mất
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2  != BLANK_TILE && val2 != STATE_MONEY)){
                //Lùi chạm đá thì giữ vị trí, x_val_ = 0 luôn
                x_pos_ = (x1 + 1)*TILE_SIZE;
                x_val_ = 0;
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
            if( (val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2  != BLANK_TILE && val2 != STATE_MONEY)){
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= (height_frame_ +1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }
         else if( y_val_ < 0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if((val1 != BLANK_TILE && val1 != STATE_MONEY) || (val2  != BLANK_TILE && val2 != STATE_MONEY)){
                y_pos_ = (y1 + 1)*TILE_SIZE;
                y_val_ = 0;
            }
        }
    }
    //XPOS CỨ TĂNG DẦN
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

    if(y_pos_ > map_data.max_y_){
        come_back_time_ = 60;
    }
}

void ThreatsObject :: ImpMoveType(SDL_Renderer* screen){
    //Đứng im 1 chỗ
    if(type_move_ == STATIC_THREAT){
        ;
    }
    //Nếu không thì di chuyển
    else{
        //Đứng trên mặt đất mới di chuyển
        if(on_ground_ == true){
            if(x_pos_ > animation_b_){
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                LoadImg("assets/img/map/threat_left.png", screen);
            }
            else if(x_pos_ < animation_a_){
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                LoadImg("assets/img/map/threat_right.png", screen);
            }
        }
        else{
            if(input_type_.left_ == 1){
                LoadImg("assets/img/map/threat_left.png", screen);
            }
            //Bổ sung
            else if(input_type_.right_ == 1){
                LoadImg("assets/img/map/threat_right.png", screen);
            }
        }
    }
}

void ThreatsObject:: InitBullet(BulletObject* p_bullet, SDL_Renderer* screen){
    if(p_bullet != NULL){
        p_bullet -> set_bullet_type(BulletObject::LASER_BULLET);
        bool ret = p_bullet -> LoadImgBullet(screen);
        if(ret == true){
            p_bullet -> set_is_move(true);
            p_bullet -> set_bullet_dir(BulletObject:: DIR_LEFT);
            //Vị trí viên đạn bay ra
            p_bullet -> SetRect (rect_.x + 5, rect_.y + 15);
            //Tốc độ đạn
            p_bullet -> set_x_val(10);
            bullet_list_.push_back(p_bullet);
        }
    }
}

void ThreatsObject :: MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit){
    for(int i = 0; i < (int) bullet_list_.size(); i++){
        BulletObject* p_bullet = bullet_list_.at(i);
        if(p_bullet != NULL){
            //Nếu được di chuyển
            if(p_bullet-> get_is_move() ){

                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;

                if(bullet_distance < 300 && bullet_distance > 0){
                    p_bullet -> HandleMove(x_limit, y_limit);
                    p_bullet -> Render (screen);

                }
                else{
                    p_bullet ->set_is_move(false);
                    
                }
            }
            //Nếu đi quá màn
            else{
                p_bullet -> set_is_move(true);
                p_bullet -> SetRect(rect_.x + 5, rect_.y +15);
            }
        }
    }
}

//x_pos_ chỉ số ứng với toàn bộ map 0 -> 25000
//rect_.x là chỉ số với màn thôi 0 ->1280

void ThreatsObject :: RemoveBullet(const int& idx){
    int size = bullet_list_.size();
    //Có ít nhất 1 viên và viên cần xóa phải là 1 trong số viên đã được bắn ra
    if(size > 0 && idx < size){
        BulletObject* p_bullet = bullet_list_.at(idx);
        bullet_list_.erase(bullet_list_.begin() + idx);
        if(p_bullet != NULL){
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

SDL_Rect ThreatsObject :: GetRectFrame(){
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}