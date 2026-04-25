#include "Window.h"

Window::Window() : event()
{
    run = true;
    width = 1000;
    height = 1000;
    window = SDL_CreateWindow("Particle_Life", width, height, SDL_WINDOW_RESIZABLE);


    //Create the window Icon
	SDL_Surface* icon = SDL_CreateSurface(1000, 1000, SDL_PIXELFORMAT_RGBA8888);
	Uint32* pixels = static_cast<Uint32*>(icon->pixels);
    for (int y = 0; y < 1000; y++) {
        for (int x = 0; x < 1000; x++) {
            if (pow(500 - x, 2)+ pow(500 - y, 2) < 250000)
            {
                pixels[y*1000+x] = 0xff0000ff;
            }
            else
            {
                pixels[y*1000+x] = 0x00000000;
            }
        }
	}

    SDL_SetWindowIcon(window, icon);
    SDL_DestroySurface(icon);
}


Window::~Window()
{
    SDL_DestroyWindow(window);
}