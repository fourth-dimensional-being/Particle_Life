#include "Renderer.h"
#include <cmath>

SDL_Texture** CreateTexturesFromColors(int typenum, int radius, std::vector<Uint32> color, SDL_Renderer* renderer)
{
    //Initialize Texture
    auto P_Textures = new SDL_Texture*[typenum];

    //Create temporary Surface to make textures
    auto surfaces = new SDL_Surface*[typenum];
    auto pixels = new Uint32*[typenum];
    for (int n = 0; n < typenum; n++) {
        surfaces[n] = SDL_CreateSurface(2*radius, 2*radius, SDL_PIXELFORMAT_RGBA8888);
        pixels[n] = static_cast<Uint32*>(surfaces[n]->pixels);
    }

    //Create Circle
    for (int y = 0; y < 2 * radius; y++) {
        for (int x = 0; x < 2 * radius; x++) {
            if (pow(radius - x, 2)+ pow(radius - y, 2) < pow(radius, 2))
            {
                for (int n = 0; n < typenum; n++)
                    pixels[n][y*2*radius+x] = color[n];

            }
            else
            {
                for (int n = 0; n < typenum; n++)
                    pixels[n][y*2*radius+x] = 0x00000000;
            }
        }
    }
    for (int n = 0; n < typenum; n++)
    {
        surfaces[n]->pixels = pixels[n];
        P_Textures[n] = SDL_CreateTextureFromSurface(renderer, surfaces[n]);
        SDL_DestroySurface(surfaces[n]);
    }
    delete[] surfaces;
    delete[] pixels;
    return P_Textures;
}
Renderer::Renderer(SDL_Window* window, const int radius, const int typenum, std::vector<Uint32> &color)
{
    //Initialize Renderer
    renderer = SDL_CreateRenderer(window, nullptr);

    P_Textures = CreateTexturesFromColors(typenum, radius, color, renderer);
}

void Renderer::Draw_Circle(float posx, float posy, int type, float zoom)
{
    const SDL_FRect rect{posx, posy, static_cast<float>(P_Textures[type]->w) / zoom, static_cast<float>(P_Textures[type]->h) / zoom};
    SDL_RenderTexture(renderer, P_Textures[type], nullptr,  &rect);
}

void Renderer::RenderSim(
    std::vector<float> posx, std::vector<float> posy,
    std::vector<int> type,
    float Width, float Height,
    float radius,
    float camx, float camy, float zoom)
{
    //SDL_RenderClear(renderer);
    for (int n = 0; n < posx.size(); n++)
    {
		float ex = (posx[n] - radius + camx) / zoom;
		float ey = (posy[n] - radius + camy) / zoom;
        if (ex < Width && posx[n] + camx + radius > 0 && ey < Height && posy[n] + camy + radius > 0)//check if particle is on screen
        {
            Draw_Circle(ex, ey, type[n], zoom);
        }
    }
    //SDL_RenderPresent(renderer);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
    delete[] P_Textures;
}
