#include "fps.h"

Fps::Fps()
{
    t = 0;
}

void Fps::Start()
{
    t = SDL_GetTicks();
}

int Fps::GetT()
{
    return SDL_GetTicks() - t;
}
