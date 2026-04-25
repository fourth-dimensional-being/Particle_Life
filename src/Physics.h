#ifndef PARTICLE_LIFE_PHYSICS_H
#define PARTICLE_LIFE_PHYSICS_H
#include <vector>
#include <string>
#include <unordered_map>
#include "Structures.h"
#include <cmath>
#include "Global.h"
class physics
{
    public:

    physics();
    float Get_Acc(float distance, int initID, int targetID); // function to calculate the acceleration between two particles
    void Apply_Physics(float &posx, float &posy, float &velx, float &vely, float &accx, float &accy, bool Torus, int TorusDimX, int TorusDimY);
    ~physics();
    float Repel_Force; //Max repel acceleration: Will be constant (Maybe)
    std::vector<std::vector<float>> Max_Force; // Max acceleration
    float Repel_Distance; //distance when acc is 0
    float Distance; // distance when acc is 0
    float Force_Radius;
    float DensityRadius;
	float MaxDensity = 100;
    float Friction;
    private:
};


#endif //PARTICLE_LIFE_PHYSICS_H