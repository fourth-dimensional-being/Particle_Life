#include <random>
#include "Particle.h"

Particle::Particle(float startx, float starty,int id) {
    ID = id;
    posx = startx;
    posy = starty;
    velx = 0;
    vely = 0;
    accx = 0;
    accy = 0;

}

Particle::~Particle()
{

}
