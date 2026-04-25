#ifndef PARTICLE_LIFE_RENDERER_H
#define PARTICLE_LIFE_RENDERER_H
#include <SDL3/SDL.h>
#include <vector>
#include <unordered_map>
#include <iostream>


SDL_Texture** CreateTexturesFromColors(int typenum, int radius, std::vector<Uint32> color, SDL_Renderer* renderer);
class Renderer
{
    public:
    SDL_Renderer* renderer;
    Renderer(SDL_Window* window, int radius, int typenum, std::vector<Uint32> &color);
    SDL_Texture** P_Textures;
    void Draw_Circle(float posx, float posy, int type, float zoom);
    void RenderSim(std::vector<float> posx, std::vector<float> posy, std::vector<int> type, float Width, float Height, float radius, float camx, float camy, float zoom);
    ~Renderer();
};

#endif //PARTICLE_LIFE_RENDERER_H