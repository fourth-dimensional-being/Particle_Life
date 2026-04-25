#ifndef PARTICLE_LIFE_PARTICLE_H
#define PARTICLE_LIFE_PARTICLE_H
#include <SDL3/SDL.h>
#include <vector>
inline int P_radius = 4;
inline int typenum = 5;
inline std::vector<Uint32> color =  {0xff0000ff, 0x00ff00ff, 0x0000ffff, 0xffff00ff, 0x00ffffff};

class Particle
{
    public:
    int ID;
    Particle(float startx, float starty, int id);
    ~Particle();
    float posx;
    float posy;
    float velx;
    float vely;
    float accx;
    float accy;

    private:




};


#endif //PARTICLE_LIFE_PARTICLE_H