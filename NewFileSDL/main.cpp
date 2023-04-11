
#include"CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
BaseObject g_background;
TTF_Font* font_time = NULL;
using namespace std;
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

        //FONT CHỮ
        if(TTF_Init() == -1){
             success = false;
        }
        font_time = TTF_OpenFont("assets/font/dlxfont_.ttf", 15);
        if(font_time == NULL){
            success = false;
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
vector<ThreatsObject*> MakeThreadList(){
    //Lưu cả quái tĩnh và quái động
    vector<ThreatsObject*> list_threats;

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

    //Các hình ảnh tĩnh in lên chỉ số
    //Mạng
    PlayerPower player_power;
    player_power.Init(g_screen);
    //Tiền
    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH*0.5 - 300 , 8);

    //Tạo quái
     vector <ThreatsObject*> threats_list = MakeThreadList();


    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("assets/img/map/exp3.png", g_screen);
    if(tRet == false){
        return -1;
    }
    exp_threat.set_clip();

    ExplosionObject exp_main;
    bool mRet = exp_main.LoadImg("assets/img/map/exp3.png", g_screen);
    if(mRet == false){
        return -1;
    }
    exp_main.set_clip();
    
    //SỐ MẠNG
    int num_die = 0;

    //Time text
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    //Bắn quái tăng điểm
    TextObject mark_game;
    mark_game.SetColor(TextObject::WHITE_TEXT);
    UINT mark_value = 0;

    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);

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

        //Draw Geometric  Vẽ Khung
        GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
        //MÀU KHUNG
        ColorData color_data(252, 79, 75);
        Geometric::RenderRectangle(rectangle_size, color_data, g_screen);
        
        GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
        ColorData color_data2(255,255,255);
        Geometric::RenderOutline(outLineSize, color_data2, g_screen);


        player_power.Show(g_screen);
        player_money.Show(g_screen);
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
                
                //Đạn của quái với nhân vật
                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                //t = threat
                vector <BulletObject*> tBullet_list = p_threat -> get_bullet_list();
                for(int jj = 0; jj < (int)tBullet_list.size(); ++jj){
                     //Lấy ra 1 phần tử
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if(pt_bullet !=NULL){
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if(bCol1 == true ){
                            //p_threat -> RemoveBullet(jj);
                            break;
                        }                        
                    } 
                } 
                // //Người chạm quái
                SDL_Rect rect_threat = p_threat -> GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
                if(bCol1 || bCol2){

                    //Vụ nổ khi nhân vật chạm đạn laser or quái
                    int frame_exp_width = exp_main.get_frame_width();
                    int frame_exp_height = exp_main.get_frame_height();
                    for(int ex = 0; ex < NUM_FRAME_EXP; ex++){
                        //Vị trí đặt vụ nổ
                        //Vụ nổ xảy ra ở GIỮA Ô HÌNH
                        //-> vị trí = đầu của ô + độ dài nửa ô
                        int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5 )- frame_exp_width * 0.5 -10;
                        int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5)- frame_exp_height * 0.5 - 20;
                        
                        exp_threat.set_frame(ex);
                        exp_threat.SetRect(x_pos, y_pos);
                        exp_threat.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }

                    num_die++;
                    if(num_die <= 3){
                        p_player.SetRect(0,0);
                        p_player.set_comeback_time(60);
                        SDL_Delay(1000);
                        player_power.Decrease();
                        player_power.Render(g_screen);
                        continue;
                    }
                    else{
                        int size = WideCharToMultiByte(CP_UTF8, 0, L"GAME OVER", -1, NULL, 0, NULL, NULL);
                        char* message = new char[size];
                        WideCharToMultiByte(CP_UTF8, 0, L"GAME OVER", -1, message, size, NULL, NULL);
                        if (MessageBoxA(NULL, message, "Info", MB_OK | MB_ICONSTOP) == IDOK){
                            p_threat ->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                    }
                }
            }
        }
        //Lấy frame là 1 ô ảnh
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        //XỬ LÍ VA CHẠM ĐẠN VÀ QUÁI
        //Lấy danh sách các viên đạn
        vector <BulletObject*> bullet_arr = p_player.get_bullet_list();
        for(int r = 0; r < (int)bullet_arr.size(); ++r){
            //Lấy từng viên ra
            BulletObject* p_bullet = bullet_arr.at(r);
            if(p_bullet != NULL){
                for(int t = 0; t < (int)threats_list.size(); ++t){
                    ThreatsObject* obj_threat = threats_list.at(t);
                    if(obj_threat != NULL){
                        //LẤY TỪNG CHỈ SỐ CỦA QUÁI
                        SDL_Rect tRect;
                        //Tọa độ bên trái 
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        //Rộng lấy 1 frame thôi
                        //Tọa độ dùng để lấy bên phải vì right = x + width. x là bên trái
                        tRect.w = obj_threat ->get_width_frame();
                        tRect.h = obj_threat -> get_height_frame();
                        //Viên đạn không có frame nên k cần lấy hết
                        SDL_Rect bRect = p_bullet ->GetRect();
                        bool bCol = SDLCommonFunc:: CheckCollision(bRect, tRect);
                        if(bCol == true){
                            mark_value++;
                            //Tạo hiệu ứng nổ
                            for(int ex = 0; ex < NUM_FRAME_EXP; ex++){
                                //Vị trí đặt vụ nổ
                                //Vụ nổ xảy ra ở GIỮA Ô HÌNH
                                //-> vị trí = đầu của ô + độ dài nửa ô
                                int x_pos = p_bullet ->GetRect().x - frame_exp_width * 0.5 + 9;
                                int y_pos = p_bullet ->GetRect().y - frame_exp_height * 0.5;
                                
                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                            }

                            //Xóa đạn
                            p_player.RemoveBullet(r);
                            obj_threat -> Free();
                            //Trung đạn thì quái chết
                            threats_list.erase(threats_list.begin() + t);
                        }  
                    }
                }   
            }
        }


        //Show Game Time
        string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() /1000;
        Uint32 val_time = 400 - time_val;
        if(val_time <= 0){
            int size = WideCharToMultiByte(CP_UTF8, 0, L"GAME OVER", -1, NULL, 0, NULL, NULL);
            char* message = new char[size];
            WideCharToMultiByte(CP_UTF8, 0, L"GAME OVER", -1, message, size, NULL, NULL);
            if (MessageBoxA(NULL, message, "Info", MB_OK | MB_ICONSTOP) == IDOK){
                is_quit = true;
                break;
            }
        }
        else{
            string str_val = to_string(val_time);
            str_time += str_val;
            
            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            //VỊ TRÍ ĐẶT TEXT
            time_game.RenderText(g_screen, SCREEN_WIDTH -150, 15);
        }
        
        //Hiển thị điểm
        string val_str_mark = to_string(mark_value);
        string strMark("Mark: ");
        strMark += val_str_mark;
        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5 , 15);

        //Hiển thị tiền
        //Bây giờ chỉ hiển thị mỗi số tiền
        //Ý định làm "Hình ảnh tiền: số tiền"
        //Chứ không làm "Money: số tiền"
        int money_count = p_player.GetMoneyCount();
        string money_str = to_string(money_count);
        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time,g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 250, 15);


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