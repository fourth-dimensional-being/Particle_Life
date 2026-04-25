#ifndef PARTICLE_LIFE_PROPERTIES_H
#define PARTICLE_LIFE_PROPERTIES_H
#include <vector>
#include "imgui.h"
#include <SDL3/SDL.h>


enum propertytype //TODO: Maybe make this not local?
{
    PARTICLE_RADIUS,
    NUMBER,
    TYPENUM,
    COLOR,
    FORCES,
    REPEL_FORCE,
    REPEL_DISTANCE,
    FORCE_DISTANCE,
    FRICTION,
    TORUS
};

struct Properties
{
    //Sim Properties
    std::vector<int> particle_dist;
    int particlenum;
    //Particle properties
    int P_radius;
    int typenum;
    std::vector<Uint32> color;
    //Physics properties
    float Repel_Force; //Max repel acceleration: Will be constant (Maybe)
    std::vector<std::vector<float>> Max_Force; // Max acceleration
    float Repel_Distance; //distance when acc is 0
    float Distance; // distance when acc is 0
    float Friction;

    //World Properties
    bool Torus;
    std::pair<int, int> TorusDim;

    //Functions
    Properties();
    void Read();
    void Write();

};
#endif //PARTICLE_LIFE_PROPERTIES_H