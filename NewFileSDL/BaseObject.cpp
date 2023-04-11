//#include "stdafx.h"
#include"BaseObject.h"
//Định nghĩa hàm constructor
using namespace std;
BaseObject::BaseObject()
{
    p_object_ = NULL;
    //Vị trí của 1 đối tượng hình ảnh hiển thị trên màn hình
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}
BaseObject::~BaseObject()
{
    Free();
}

bool BaseObject::LoadImg(string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;
    //Hàm IMG_Load sẽ trả ra 1 surface
    //Và dùng surface để load lên màn hình
    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if(load_surface != NULL){
        //Khi màu này được thiết lập thì các pixel cùng màu với nó sẽ không được in ra
        //Xóa nền
        //SDL_MapRBG chuyển đổi giá trị màu từ không gian RBG sang giá trị màu 32bit in ra màn
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        //Tạo một đối tượng Texture từ 1 đối tượng surface
        //Surface chứa thông tin về định dạng màu săc của ảnh
        //Texture là đối tượng dùng để hiện thị ảnh(lưu giữ toàn bộ thông tin tấm ảnh)
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if(new_texture !=NULL){
            //Tấm ảnh 1 frame = 64x64 = TILE_MAP 
            //NẾU 1 Ô LÀ 64X64 THÌ FRAME <=   64X64 ĐỂ CÓ THỂ RƠI KHI ĐI VÀO Ô KHÔNG CÓ GÌ
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    } 
    p_object_ = new_texture;
    return p_object_ != NULL;
}
//SDL_Render* des: load ảnh lên nền này 
void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip /*=NULL*/)
{
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
    //Sao chép 1 phần texture vào mục tiêu hiện tại của renderer
    //des: nơi sd  để sao chép, p_object: con trỏ tới texture muốn sao chép
    //clip: con trỏ tới 1 vùng của texture muốn sao chép
    //renderquad: con trỏ tới 1 vùng cửa sổ muốn sao chép vào
    //->đẩy thông số của p_object_ lên des
    SDL_RenderCopy(des, p_object_, clip, &renderquad);
}
void BaseObject::Free()
{
    if(p_object_ != NULL){
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}