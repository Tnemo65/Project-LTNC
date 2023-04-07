
#include"CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
BaseObject g_background;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0){
        return false;
    }
    //Thiết lập chất lượng hình ảnh
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   
    g_window = SDL_CreateWindow("GAME", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, 
                                SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN);
    if(g_window == NULL){
        success = false;
    }
    else{
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL){
            success = false;
        }
        else{
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags)){
                success = false;
            }
        }
    }
    return success;
}

bool LoadBackground()
{   
    bool ret = g_background.LoadImg("assets/img/map/background.png", g_screen);
    if(ret == false){
        return false;
    }
    return true;
}          

void close(){ 
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}
std::vector<ThreatsObject*> MakeThreadList(){
    //Lưu cả quái tĩnh và quái động
    std::vector<ThreatsObject*> list_threats;

    //QUÁI DI CHUYỂN ĐỘNG
    ThreatsObject* dynamic_threats = new ThreatsObject[20];
    for(int i = 0; i < 20; i++){
        ThreatsObject* p_threat = (dynamic_threats + i);
        if(p_threat != NULL){
            p_threat -> LoadImg("assets/img/map/threat_left.png",g_screen);
            p_threat -> set_clips();
            p_threat -> set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat -> set_x_pos (500 + i * 500);
            p_threat -> set_y_pos(200);

            int pos1 = p_threat -> get_x_pos() - 60;
            int pos2 = p_threat -> get_x_pos() + 60;
            p_threat -> SetAnimationPos(pos1, pos2);
            p_threat -> set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }



    //SỐ LƯỢNG QUÁI
    //QUÁI ĐỨNG IM
    ThreatsObject* threats_objs = new ThreatsObject[20];
    for(int i = 0; i < 20; i++){
        ThreatsObject* p_threat = (threats_objs +i);
        if(p_threat != NULL){
            p_threat -> LoadImg("assets/img/map/threat_level.png", g_screen);
            p_threat -> set_clips();
            //Đặt vị trí quái threat
            p_threat -> set_x_pos(700 + i*1200 - 50);
            p_threat -> set_y_pos(250);
            p_threat -> set_type_move(ThreatsObject:: STATIC_THREAT);
            //Để nó không di chuyển
            p_threat -> set_input_left(0);
            
            //Thêm đạn cho quái đứng im
            BulletObject* p_bullet = new BulletObject();
            p_threat -> ThreatsObject::InitBullet (p_bullet, g_screen);
            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[]){
    ImpTimer fps_timer;

    if(InitData() == false){
        return -1;
    }
    if(LoadBackground() == false){
        return -1;
    }

    GameMap game_map;
    //fill ảnh đất
    //Đọc map viết bởi số 0 1 2 
    char file_path[] = "assets/img/map/map01.dat";
    game_map.LoadMap(file_path);
    //Load các hình ảnh tương ứng vào ô 1 2 3
    game_map.LoadTiles(g_screen);

    //Khai báo và lấy hình ảnh nhân vật ban đầu là sang phải
    MainObject p_player;
    p_player.LoadImg("assets/img/map/player_right.png", g_screen);
    p_player.set_clips();

    //Tạo quái
    std:: vector <ThreatsObject*> threats_list = MakeThreadList();

    bool is_quit = false;
    while(!is_quit){
        fps_timer.start();
        while(SDL_PollEvent( &g_event) != 0){
            if( g_event.type == SDL_QUIT){
                is_quit = true;
            }
            //Để lấy trạng thái các nút mình bấm trái phải
            p_player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        //Vẽ map
        Map map_data = game_map.getMap();
        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);
        p_player.Show(g_screen); 

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        //Xử lí quái
        for(int i = 0; i < (int)threats_list.size(); i++){
            //Tạo list quái
            ThreatsObject* p_threat = threats_list.at(i);
            if(p_threat != NULL){
                p_threat -> SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat -> ImpMoveType(g_screen);
                p_threat -> DoPlayer(map_data);
                p_threat -> MakeBullet(g_screen,SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat -> Show(g_screen);
            }
        }
        SDL_RenderPresent(g_screen);
        //Thời gian thực sự trôi qua
        int real_imp_time = fps_timer.get_ticks();
        //Thời gian chạy 1 frame
        int time_one_frame = 1000/FRAME_PER_SECOND; //ms
        //Thời gian chạy thực tế < thời gian lí thuyết
        
        //Nếu delay_time to-> ctrinh chậm dần
        //delay_time to khi Frame_per_second nhỏ
        //Để ctrinh chạy nhanh -> tăng FPS
        if(real_imp_time < time_one_frame){
                int delay_time = time_one_frame - real_imp_time;
                if(delay_time >= 0){
                    SDL_Delay(delay_time);
                }
        }

    }    

    for(int i = 0; i < (int)threats_list.size(); i++){
        ThreatsObject* p_threat = threats_list.at(i);
        if(p_threat != NULL){
            p_threat -> Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();
    close();
    return 0;
}