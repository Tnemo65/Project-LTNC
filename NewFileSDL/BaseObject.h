#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "CommonFunc.h"

class BaseObject
{
    public:
        BaseObject();
        ~BaseObject();
        //Hàm set kích thước cho rect
        void SetRect(const int& x, const int& y){
            //Vị trí của 1 đối tượng hình ảnh hiển thị trên màn hình
            rect_.x = x;
            rect_.y = y;
        }
        //Hàm lấy ra kích thước của rect
        SDL_Rect GetRect() const { return rect_;}
        //Hàm lấy ra p_object
        SDL_Texture* GetObject() const { return p_object_;}
        //Hàm load ảnh bitmap lên đối tượng p_object
        //Rồi dùng p_object gán lên màn hình
        //VIRTUAL để làm gì -> để có thể tái sử dụng mà không cần đúng phải là đối tượng trong class đó
        virtual bool LoadImg(std:: string path, SDL_Renderer* screen);
        void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void Free();
    protected:
        SDL_Texture* p_object_; //Biến sử dụng để lưu trữ hình ảnh
        SDL_Rect rect_; //Biến sử dụng để lưu trữ kích thước ảnh
};

#endif