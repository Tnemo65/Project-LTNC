#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunc.h"
using namespace std;
class TextObject{
public:
    TextObject();
    ~TextObject();
    
    enum TextColor{
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
        BLUE_TEXT = 3,
        GREEN_TEXT = 4,
    };
    //Laychu
    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

    //CaiMau
    void SetColor(int type);

    //XuatChu
    void RenderText(SDL_Renderer* screen,
                    int xp, int yp,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE );
    int GetWidth() const { return width_;}
    int GetHeight() const { return height_;}
        //CaiNoiDung
       void SetText (const string& text) { str_val_ = text;}
    string GetText() const { return str_val_;}
private:
    string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    int width_;
    int height_;
};
#endif