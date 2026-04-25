#include "RNG.h"


RNG::RNG() : rand_seed(std::time(NULL)), engine(rand_seed){};

float RNG::Get_Random(float L, float U){
    std::uniform_real_distribution<float> dist(L, U);
    return dist(engine);
}