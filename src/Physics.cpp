#include "Physics.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "Particle.h"

physics::physics() : Max_Force(
{
    {
        {30, -30, 20, 20, 10}, //the acceleration of init toward target. Init y axis, Target X axis
        {30, 30, -10, -20, 15},
        {30, 0, -30, -30, -25},
        {30, -20, 30, -5, 30},
        {30, 15, -10, 0, -10}
      }
   })
{
    
    Friction = .977;
    Repel_Force = -66.66;
    Repel_Distance = 20;
    Distance = 66.66;
    Force_Radius = (Distance-Repel_Distance)/2;
    DensityRadius = std::abs(Repel_Force);
    MaxDensity = 20;
}


float physics::Get_Acc(float distance, int initID, int targetID)
{
    if (distance < Repel_Distance)
    {
        return Repel_Force / Repel_Distance * (Repel_Distance - distance);
    }
    if (distance < Distance)
    {
        return Max_Force[initID][targetID] * (1 - std::abs(distance - Force_Radius - Repel_Distance) / Force_Radius);
    }
    return 0;
}

void physics::Apply_Physics(float &posx, float &posy, float &velx, float &vely, float &accx, float &accy, bool Torus, int TorusDimX, int TorusDimY)
{
    velx = Friction * velx + accx/static_cast<float>(Frame_Rate);
    vely = Friction * vely + accy/static_cast<float>(Frame_Rate);
    posx += velx/static_cast<float>(Frame_Rate);
    posy += vely/static_cast<float>(Frame_Rate);
    if (Torus)
    {
        posx = (posx < 0) ? TorusDimX + std::fmod(posx, (float)TorusDimX) : std::fmod(posx, (float)TorusDimX);
        posy = (posy < 0) ? TorusDimY + std::fmod(posy, (float)TorusDimY) : posy = std::fmod(posy, (float)TorusDimY);
    }
}
physics::~physics()
{

}

