#ifndef PARTICLE_LIFE_WINDOW_H
#define PARTICLE_LIFE_WINDOW_H
#include <SDL3/SDL.h>
#include <cmath>
class Window
{
    public:
    bool run;
    int width;
    int height;
    SDL_Window* window;
    SDL_Event event;

    Window();
    ~Window();
    private:
};


#endif //PARTICLE_LIFE_WINDOW_H