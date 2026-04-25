#ifndef PARTICLE_LIFE_SIM_H
#define PARTICLE_LIFE_SIM_H
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Particle.h"
#include "Renderer.h"
#include "RNG.h"
#include "Physics.h"
#include "Window.h"
#include "Camera.h"
#include "GUI.h"
#include <vector>
#include <array>
#include <fstream>
#include "Properties.h"
#include "Structures.h"


class Sim
{
    public:
    //variables
    std::vector<int> particle_dist;
    int particle_num;
    bool run;
    UI_settings settings;
    bool mouse_down; //I genuinely dont know what to do with this but this has to have a better solution. Make it go in window at he very least
    bool Torus;
    std::pair<int, int> TorusDim;
   
    //objects
    std::vector<Particle> particles;

    //Engines
    Window win;
    Camera cam;
    RNG rng;
    Renderer renderer;
    physics Physics_Engine;

    //Functions
    Sim();
    std::array<std::vector<float>, 2> Get_Coords(); //turn this into VEC2
    std::array<std::vector<float>, 2> Get_Vel(); //TODO: MAKE THESE INTO ONE FUNCTION
    std::array<std::vector<float>, 2> Get_Acc();
    std::vector<int> Get_Types();
    void Get_Force(int threadnum, int threadtotal); // function to calculate the force between two particles //TODO: maybe move this to physics somehow???
    void Move_Step();
    ~Sim();
    void Pull_Event();
    void Restart_Simulation();
    void Update_Properties();
    private:

};



#endif //PARTICLE_LIFE_SIM_H