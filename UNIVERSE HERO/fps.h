#ifndef FPS_H_INCLUDED
#define FPS_H_INCLUDED

#include "CommonFunc.h"

class Fps
{
public:
    Fps();
    ~Fps(){}

    void Start();
    int GetT();

private:
    int t;
};

#endif // FPS_H_INCLUDED
