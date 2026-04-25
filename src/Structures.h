#ifndef PARTICLE_LIFE_STRUCTURES_H
#define PARTICLE_LIFE_STRUCTURES_H
#include <vector>
struct Vec2
{
    float x, y;
    Vec2(float X, float Y) : x(X), y(Y) {};
};

struct Rect
{
    float x, y, w, h;
    Rect(float X, float Y, float W, float H) : x(X), y(Y), w(W), h(H) {};
};

struct UI_settings
{
    bool pause;
    bool temppause;
    bool Setting_display;
    bool Display_init;

    UI_settings() : pause(false), temppause(false), Setting_display(false), Display_init(false) {};
};

#endif //PARTICLE_LIFE_STRUCTURES_H