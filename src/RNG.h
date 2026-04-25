#ifndef PARTICLE_LIFE_RNG_H
#define PARTICLE_LIFE_RNG_H
#include <random>
#include <ctime>

class RNG
{
private:
    unsigned long rand_seed;
    std::default_random_engine engine;
public:
    RNG();
    float Get_Random(float L, float U);
};
#endif //PARTICLE_LIFE_RNG_H
