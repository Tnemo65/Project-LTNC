#include"MainObject.h"
using namespace std;
MainObject::MainObject()
{

    frame_ = 0;
    //Tọa độ của nhân vật ...
    x_pos_ = TILE_SIZE;
    y_pos_ = 0;
    //Giá trị tăng của x y khi di chuyển
    x_val_ = 0;
    y_val_ = 0;
    //Chỉ số của khung hình (1 ô)
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE; //chưa biết trái phải
    //Kiểm tra xem nhập từ bên ngoài hướng nào
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    double_jump_ = true;

    input_type_.down_ =0;
    input_type_.up_ = 0; 
    //Kiểm tra có va chạm không
    on_ground_ = false;

    map_x_ = 0;
    map_y_ = 0;
    //Thời gian quay lại
    come_back_time_ = 0;
    //Đếm tiền
    money_count = 0;
    IsIncreaseLife = false;
    IsDecreaseLife = false;

}

MainObject::~MainObject()
{

}

//Tải 1 hình ảnh và lấy kích thước ảnh
bool MainObject::LoadImg(string path, SDL_Renderer* screen){
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret==true){
        //THAY ĐỔI FRAME Ở ĐÂY THAY 8 = 16
        //Làm cho chiều rộng của 1 frame là 60 < TileMap(kích thước 1 ô)
        width_frame_ = rect_.w /8;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect MainObject :: GetRectFrame(){
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = rect_.x/8;
    rect.h = rect_.h;
    return rect;
}


//Thiết lập hình cắt 
void MainObject::set_clips(){
    //Kiểm tra kích thước được thiết lập chưa
    if(width_frame_ >0 && height_frame_ >0){
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

void MainObject:: Show(SDL_Renderer* des){
    UpdateImagePlayer(des);
    //Nếu liên tục bấm -> tăng frame
    if(input_type_.left_ == 1 ||
       input_type_.right_ == 1||
       input_type_.stand_ == 1)
    {
        frame_ ++;
    }
    //Nếu không bấm thì giữ frame = 0
    else{
        frame_ = 0;
    }

    //Nếu như chạy hết 8 frame rồi -> quay lại
    if(frame_ >= 8){
        frame_ = 0;
    }
    
    if(come_back_time_ == 0){
        //Lấy vị trí và kích thước của hình ảnh tại frame hiện tại
        //TRỪ ĐI ĐỂ NHÂN VẬT ĐỨNG ĐÚNG CHỖ TRÊN MÀN HÌNH
        //NẾU KHÔNG THÌ NHÂN VẬT SẼ BỊ CUỐN THEO MÀN HÌNH
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        //Lấy current clip tại frame đang chạy 
        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        //Vẽ đối tượng 
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }

}

//Kiểm tra phím nào được ấn và đánh dấu phím đó
void MainObject:: HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk*bullet_sound[2]){
    if(events.type == SDL_KEYDOWN) {
        switch(events.key.keysym.sym){
        case SDLK_RIGHT:
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                UpdateImagePlayer(screen);
            }
            break;
        
        case SDLK_LEFT:
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                UpdateImagePlayer(screen);
            }
            break;


        case SDLK_UP:
            {
                input_type_.jump_ = 1;
                UpdateImagePlayer(screen);
            }
        break;
        }
    }

    //Nếu như nhả phím
    else if(events.type == SDL_KEYUP){
        switch(events.key.keysym.sym){
        case SDLK_RIGHT:
            {
                input_type_.right_ = 0;
            }
            break;
        
        case SDLK_LEFT:
            {
                input_type_.left_ = 0;
            }
            break;
        case SDLK_UP:
        {
            input_type_.jump_ = 0;
        }
        break;
        }
    }
     if(events.type == SDL_MOUSEBUTTONDOWN){
        if (events.button.button == SDL_BUTTON_RIGHT){
            BulletObject* p_bullet = new BulletObject();
            //THAY ĐỔI LOẠI ĐẠN
            p_bullet -> set_bullet_type(BulletObject::LASER_BULLET1);
            p_bullet -> LoadImgBullet(screen);
            //XÁC ĐỊNH ĐẦU RA CỦA ĐẠN
            if(status_ == WALK_LEFT){
                    //Set đang quay sang trái hay phải
                    p_bullet -> set_bullet_dir(BulletObject::DIR_LEFT);
                    //Lấy vị trí hình ảnh viên đạn theo nhân vật
                    p_bullet ->SetRect(this ->rect_.x + width_frame_ - TILE_SIZE, rect_.y + height_frame_ * 0.48);            
                    Mix_PlayChannel(-1, bullet_sound[1], 0);

            }
            else if(status_ == WALK_RIGHT){
                    p_bullet -> set_bullet_dir(BulletObject::DIR_RIGHT);
                    p_bullet ->SetRect(this ->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.48);            
                    Mix_PlayChannel(-1, bullet_sound[1], 0);

            }
            //Set tốc độ bắn
            p_bullet -> set_x_val(BULLET_VAL);
            p_bullet -> set_y_val(BULLET_VAL);
            //Trong map
            p_bullet -> set_is_move(true);
            //Nạp đạn vào băng
            p_bullet_list_.push_back(p_bullet);

        }
        else if(events.button.button == SDL_BUTTON_LEFT){
            BulletObject* p_bullet = new BulletObject();
            //THAY ĐỔI LOẠI ĐẠN
            p_bullet -> set_bullet_type(BulletObject::SPHERE_BULLET);
            p_bullet -> LoadImgBullet(screen);
            //XÁC ĐỊNH ĐẦU RA CỦA ĐẠN
            if(status_ == WALK_LEFT){
                    //Set đang quay sang trái hay phải
                    p_bullet -> set_bullet_dir(BulletObject::DIR_LEFT);
                    //Lấy vị trí hình ảnh viên đạn theo nhân vật
                    p_bullet ->SetRect(this ->rect_.x + width_frame_ - TILE_SIZE, rect_.y + height_frame_ * 0.22);            
                    Mix_PlayChannel(-1, bullet_sound[0], 0);
                    // p_bullet ->CheckToMap(map_data);

            }
            else if(status_ == WALK_RIGHT){
                    p_bullet -> set_bullet_dir(BulletObject::DIR_RIGHT);
                    p_bullet ->SetRect(this ->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.22);            
                    Mix_PlayChannel(-1, bullet_sound[0], 0);
                    // p_bullet ->CheckToMap(map_data);


            }
            //Set tốc độ bắn
            p_bullet -> set_x_val(BULLET_VAL);
            p_bullet -> set_y_val(BULLET_VAL);
            //Trong map
            p_bullet -> set_is_move(true);
            //Nạp đạn vào băng
            p_bullet_list_.push_back(p_bullet);

        }
    }
}

void MainObject::HandleBullet(SDL_Renderer* des, Map& map_data){
    for(int i = 0; i < (int)p_bullet_list_.size(); i++){
        BulletObject* p_bullet = p_bullet_list_.at(i);

        if(p_bullet != NULL){

            //Nếu vẫn trong map
            if(p_bullet -> get_is_move() == true){
                p_bullet ->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet -> Render(des);
            }
            //Nếu ra ngoài map thì xóa đạn đi
            else{
                p_bullet_list_.erase(p_bullet_list_.begin() +i);
                if(p_bullet != NULL){
                    //delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

//Di chuyển sang trái phải, rơi tự do
void MainObject::DoPlayer(Map &map_data){
    //Nếu như nhân vật ở trong bản đồ thì xử lí, ngoài bản đồ thì thôi
    if(come_back_time_ == 0){
        x_val_ = 0;
        //Khi y_val_ cộng lên dần thì nhân vật rơi dần từ trên xuống
        //Tốc độ rơi
        y_val_ += GRAVITY_SPEED;
        //Chỉ tăng đến 1 lượng nào đó
        if(y_val_ >= MAX_FALL_SPEED){
            y_val_ = MAX_FALL_SPEED;
        }
        if(input_type_.left_ == 1){
            x_val_ -= PLAYER_SPEED;;
        }
        else if(input_type_.right_ == 1){
            x_val_ += PLAYER_SPEED;
        }
        if (input_type_.jump_ == 1 && on_ground_ == true) {
            y_val_ = -PLAYER_JUMP_VAL;
            on_ground_ = false;
            input_type_.jump_ = 0;
        } else if (input_type_.jump_ == 1 && on_ground_ == false && double_jump_ == true) {
            y_val_ = -PLAYER_JUMP_VAL;
            double_jump_ = false;
            input_type_.jump_ = 0;
        }
        if (on_ground_ == true) {
            double_jump_ = true;
        }
        //Để đứng trên đất mà không rơi qua đất 
        CheckToMap(map_data);
        CenterEntityOnMap(map_data);
    }
    if(come_back_time_ > 0){
        come_back_time_ --;
        if(come_back_time_ == 0){
            on_ground_ = false;
            if(x_pos_ > 256){
                //4 TILE map
                x_pos_ -= 256;
            }
            else{
                x_pos_ = 0;
            }
            //Nếu mà hết thời gian chờ thì nvat rơi từ trên xuống
            //Ở trên cùng map
            y_pos_ = TILE_SIZE;
            x_val_ = 0;
            y_val_ = 0;
        }
    }
}

void MainObject::CenterEntityOnMap(Map& map_data){
    //Nhân vật di chuyển nửa bản đồ thì di chuyển bản đồ
    //x_pos_ giữa map, SCREEN_WIDTH/2 nửa map -> map_data.start_x_ là 0
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/2) ;
    //Nếu mà cứ lùi thì k cho quay lại màn hình
    if(map_data.start_x_ < 0){
        map_data.start_x_ = 0;
    }
    else if( map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_){
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }
    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT/2);
    if(map_data.start_y_ < 0) {
        map_data.start_y_ = 0;
    }
    else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_){
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}

//Làm cho nhân vật bị chặn khi gặp chướng ngại vật
void MainObject:: CheckToMap(Map &map_data){
    //Định vị trí đầu cuối
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

                        //KIỂM TRA THEO CHIỀU NGANG
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
            if(val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else if(val1 == STATE_GAI1 || val2 == STATE_GAI1 || val2 == STATE_GAI2 || val1 == STATE_GAI2){
                IsDecreaseLife  = true;
            }
            else if (val1 == STATE_DIAMOND || val2 == STATE_DIAMOND){
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                for(int i = 0; i < 25; i++){
                    IncreaseMoney();
                }
            }
            else if(val1 == STATE_LIFE || val2 == STATE_LIFE){
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                IsIncreaseLife = true;
                Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/game/thuthapmang.wav"), 0);

            }
            
            else if (val1 == 27 || val2 == 27){
                int mark1 = GetMarkCount();
                int money =GetMoneyCount();
                if(mark1 >= 30 && money >= 300){
                    win = true;
                }
            }
            //Nếu không phải ô tiền và nếu ô không phải rỗng thì bị chặn
            else{
                //Kiểm tra xem ô bên phải có phải là ô trống hay không
                if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18)){
                    x_pos_ = x2 * TILE_SIZE; // Ra vị trí biên của frame nhân vật
                    //Nếu mà chạm tường thì x_pos_ chỉ dừng ở đó
                    x_pos_ -= width_frame_ + 1;
                    x_val_ = 0;
                }
            }
        }
        //di chuyển sang trái
        else if (x_val_ < 0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            //Nếu ăn phải tiền thì ô tiền biến mất
            if(val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                IncreaseMoney();
            }
            else if (val1 == STATE_DIAMOND || val2 == STATE_DIAMOND){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                for(int i = 0; i < 25; i++){
                    IncreaseMoney();
                }
            }
            else if(val1 == STATE_GAI1 || val2 == STATE_GAI1 || val2 == STATE_GAI2 || val1 == STATE_GAI2){
                IsDecreaseLife  = true;
            }
            else if(val1 == STATE_LIFE || val2 == STATE_LIFE){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                IsIncreaseLife = true;
                Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/game/thuthapmang.wav"), 0);

            }
            else if (val1 == 27 || val2 == 27){
                int mark1 = GetMarkCount();
                int money =GetMoneyCount();
                if(mark1 >= 30 && money >= 300){
                    win = true;
                }
            }

            else{
                if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18)){
                    //Lùi chạm đá thì giữ vị trí, x_val_ = 0 luôn
                    x_pos_ = (x1 + 1)*TILE_SIZE;
                    x_val_ = 0;
                }
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
            if(val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else if (val1 == STATE_DIAMOND || val2 == STATE_DIAMOND){
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                for(int i = 0; i < 25; i++){
                    IncreaseMoney();
                }
            }
            else if(val1 == STATE_GAI1 || val2 == STATE_GAI1 || val2 == STATE_GAI2 || val1 == STATE_GAI2){
                IsDecreaseLife  = true;
            }
            else if(val1 == STATE_LIFE || val2 == STATE_LIFE){
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                IsIncreaseLife = true;
                Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/game/thuthapmang.wav"), 0);


            }
            else if (val1 == 27 || val2 == 27){
                int mark1 = GetMarkCount();
                int money =GetMoneyCount();
                if(mark1 >= 30 && money >= 300){
                    win = true;
                }
            }

            else{
                if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18)){
                    y_pos_ = y2 * TILE_SIZE;
                    y_pos_ -= (height_frame_ +1);
                    y_val_ = 0;
                    on_ground_ = true;
                    if(status_ == WALK_NONE){
                        status_ = WALK_RIGHT;
                    }
                }
            }
        }
         else if( y_val_ <0){
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if(val1 == STATE_MONEY || val2 == STATE_MONEY){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                IncreaseMoney();
            }
            else if (val1 == STATE_DIAMOND || val2 == STATE_DIAMOND){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                for(int i = 0; i < 25; i++){
                IncreaseMoney();
                }
            }
            else if(val1 == STATE_GAI1 || val2 == STATE_GAI1 || val2 == STATE_GAI2 || val1 == STATE_GAI2){
                IsDecreaseLife  = true;
            }
            else if(val1 == STATE_LIFE || val2 == STATE_LIFE){
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                IsIncreaseLife = true;
                Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/game/thuthapmang.wav"), 0);

            }
            else if (val1 == 27 || val2 == 27){
                int mark1 = GetMarkCount();
                int money =GetMoneyCount();
                if(mark1 >= 30 && money >= 300){
                    win = true;
                }
            }

            else{
                if((val1 >= 1 && val1 <= 18) || (val2 >= 1 && val2 <=18) ){
                     y_pos_ = (y1 + 1)*TILE_SIZE;
                    y_val_ = 0;
                }
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

    if(y_pos_ >= map_data.max_y_ - TILE_SIZE/3){
        IsDecreaseLife = true;
        come_back_time_ = 60;
    }
}

void MainObject::IncreaseMoney(){
    money_count ++;
    Mix_PlayChannel(-1, Mix_LoadWAV("assets/sound/game/thuthaptien.wav"), 0);
}



void MainObject:: UpdateImagePlayer(SDL_Renderer * des){
    //Nếu dưới đất lấy ảnh dưới đất
    if(on_ground_ == true){
        //Nếu là trái phải thì lấy ảnh trái phải
        if(status_ == WALK_LEFT){
            LoadImg("assets/img/map/player_left.png", des);
        } 
        else if(status_ == WALK_RIGHT){
            LoadImg("assets/img/map/player_right.png", des);
        }

    }
    //Nếu trên không trung lấy ảnh trên không trung
    else{
        //Mà ở đầu map thì load ảnh nhảy bên phải
        if(x_pos_ == TILE_SIZE){
            LoadImg("assets/img/map/jum_right.png", des);
        }
        //Nếu là trái phải thì lấy ảnh trái phải
        if(status_ == WALK_LEFT){
            LoadImg("assets/img/map/jum_left.png", des);
        }
        else if(status_ == WALK_RIGHT){
            LoadImg("assets/img/map/jum_right.png", des);
        }
    } 
}

void MainObject :: RemoveBullet(const int& idx){
    int size = p_bullet_list_.size();
    //Có ít nhất 1 viên và viên cần xóa phải là 1 trong số viên đã được bắn ra
    if(size > 0 && idx < size){
        BulletObject* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);
        if(p_bullet){
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
