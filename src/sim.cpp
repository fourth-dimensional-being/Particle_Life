#include <iostream>
#include <thread>
#include <algorithm>
#include "Sim.h"

struct hasher //TODO: What the fuck do I even do with this??
{
    std::size_t operator() (const std::pair<int , int> &pair) const
    {
        auto h1 = std::hash<int>{}(pair.first);
        auto h2 = std::hash<int>{}(pair.second);

        return h1^h2;

    }
};

struct Triangle {
    float distance;
    float dx;
    float dy;

    Triangle(float distance, float dx, float dy) : distance(distance), dx(dx), dy(dy) {}
};

Sim::Sim() :
    rng(),
    win(),
    renderer(win.window, P_radius, typenum, color),
    Physics_Engine(),
    settings()

{
    particle_num = 0;
    particle_dist = {500, 500, 500, 500, 500};
    run = true;
	mouse_down = false;
    for (int n =0; n < particle_dist.size(); n++) {
        particle_num += particle_dist[n];
        for (int i = 0; i < particle_dist[n]; i++) {
            particles.emplace_back(
                rng.Get_Random(0, static_cast<float>(win.width)),
                rng.Get_Random(0, static_cast<float>(win.height)),
                n);
        }
    }
    Torus = 0;
    TorusDim = {win.width, win.height};

    //init ImGui
    //TODO: Consider making this a function in GUI.h
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(win.window, renderer.renderer);
    ImGui_ImplSDLRenderer3_Init(renderer.renderer);

    //create properties file
    std::ofstream file;
	file.open("Properties.txt");

    //write all properties to file
    file << "PARTICLE_RADIUS\n";
    file << P_radius << std::endl;

	file << "NUMBER\n";
    for (int n : particle_dist)
        file << n << std::endl;

    file << "TYPENUM\n";
    file << particle_dist.size() << std::endl;

    file << "COLOR\n";
    for (int n = 0; n < particle_dist.size(); n++)
		file << color[n] << std::endl;

	file << "FORCES\n";
    for (int init = 0; init < particle_dist.size(); init++) {
        for (int target = 0; target < particle_dist.size(); target++) {
			file << Physics_Engine.Max_Force[init][target] << std::endl;
        }
    }

    file << "REPEL_FORCE\n";
    file << Physics_Engine.Repel_Force<<std::endl;

    file << "REPEL_DISTANCE\n";
    file << Physics_Engine.Repel_Distance<<std::endl;

    file << "FORCE_DISTANCE\n";
    file << Physics_Engine.Distance<<std::endl;

    file << "FRICTION\n";
    file << Physics_Engine.Friction<<std::endl;

    file <<"TORUS\n";
    file << Torus<<std::endl;
    file << TorusDim.first<<std::endl;
    file << TorusDim.second<<std::endl;
            

}

std::array<std::vector<float>, 2> Sim::Get_Coords() //TODO: This is duplicated code, try making it one function
{
    std::vector<float> x;
    std::vector<float> y;
    for (int n = 0; n < particle_num; n++) {
        x.push_back(particles[n].posx);
        y.push_back(particles[n].posy);
    }
    return std::array {x,y};
}

std::array<std::vector<float>, 2> Sim::Get_Vel()
{
    std::vector<float> x;
    std::vector<float> y;
    for (int n = 0; n < particle_num; n++) {
        x.push_back(particles[n].velx);
        y.push_back(particles[n].vely);
    }
    return std::array {x,y};
}

std::array<std::vector<float>, 2> Sim::Get_Acc()
{
    std::vector<float> x;
    std::vector<float> y;
    for (int n = 0; n < particle_num; n++) {
        x.push_back(particles[n].accx);
        y.push_back(particles[n].accy);
    }
    return std::array {x,y};
}

std::vector<int> Sim::Get_Types()
{
    std::vector<int> types;
    for (int n = 0; n < particle_num; n++)
        types.push_back(particles[n].ID);
    return types;
}

void Sim::Get_Force(int threadnum, int threadtotal) {
    for (int init = threadnum * particle_num/threadtotal; init < (threadnum+1) * particle_num / threadtotal; init++) {
        float accx = 0;
        float accy = 0;
        float initx = particles[init].posx;
        float inity = particles[init].posy;
        for (int target = 0; target < particle_num; target++) {
            if (init != target) { //TODO: Maybe move some of these operations into physics.cpp
                float dx = particles[target].posx - initx;
                if (std::abs(dx) < Physics_Engine.Distance) {
                    float dy = particles[target].posy - inity;
                    if (std::abs(dy) < Physics_Engine.Distance) {
                        float distance = sqrtf(dx * dx + dy * dy);
                        float acc = Physics_Engine.Get_Acc(distance, particles[init].ID, particles[target].ID);
                        float acctodratio = acc / distance;
                        accx += acctodratio * dx;
                        accy += acctodratio * dy;
                    }
                }
            }
        }
        particles[init].accx = accx;
        particles[init].accy = accy;
    }
}

void Sim::Move_Step()
{
    /*
    std::vector<std::thread> threads;
    for (int n = 0; n < 10; n++) {
        threads.push_back(std::thread(&Sim::Get_Force, this, n, 10));
    }
    for (auto &t : threads)
        t.join();
    */




    //update acceleration for all particles

    std::unordered_map<std::pair<int, int>, std::vector<Particle*>, hasher> hashmap;
    for (int n = 0; n < particle_num; n++)
    {
        int mapx = (int)floor(particles[n].posx/Physics_Engine.Distance);
        int mapy = (int)floor(particles[n].posy/Physics_Engine.Distance);
        hashmap[std::pair(mapx, mapy)].push_back(&particles[n]);
        if (Torus)
        {
            if (particles[n].posx < 2*Physics_Engine.Distance - fmod(TorusDim.first, Physics_Engine.Distance))
            {
                hashmap[std::pair((int)floor((particles[n].posx + (float)TorusDim.first) / Physics_Engine.Distance), (int)floor((particles[n].posy + (float)TorusDim.second) / Physics_Engine.Distance))].push_back(&particles[n]);
            }
            if (particles[n].posx > (float)TorusDim.first - Physics_Engine.Distance)
            {
                hashmap[std::pair((int)floor((particles[n].posx - (float)TorusDim.first) / Physics_Engine.Distance), (int)floor((particles[n].posy - (float)TorusDim.second) / Physics_Engine.Distance))].push_back(&particles[n]);
            }

            if (particles[n].posy < 2*Physics_Engine.Distance - fmod(TorusDim.second, Physics_Engine.Distance))
            {
                hashmap[std::pair((int)floor((particles[n].posx + (float)TorusDim.first) / Physics_Engine.Distance), (int)floor((particles[n].posy + (float)TorusDim.second) / Physics_Engine.Distance))].push_back(&particles[n]);
            }
            if (particles[n].posy > (float)TorusDim.second - Physics_Engine.Distance)
            {
                hashmap[std::pair((int)floor((particles[n].posx - (float)TorusDim.first) / Physics_Engine.Distance), (int)floor((particles[n].posy - (float)TorusDim.second) / Physics_Engine.Distance))].push_back(&particles[n]);
            }
        }
    }


    for (auto& pair : hashmap)
    {
        for (Particle* n : pair.second)
        {
            float accx = 0;
            float accy = 0;
            float local_density = 0;
            float initx = n->posx;
            float inity = n->posy;

            //Get Local Density and Local Particle Information
            std::vector <std::pair <Particle*, Triangle>> local_particles;

            for (int y = -1; y < 2; y++) 
            {
                for (int x = -1; x < 2; x++) 
                {
                    std::pair pair2 = { pair.first.first + x, pair.first.second + y };
                    if (hashmap.count(pair2)) 
                    {
                        float dx, dy;
                        for (Particle* P : hashmap[pair2]) {
                            if (Torus)
                            {
                                if (pair2.first < 0)
                                {
                                    dx = TorusDim.first - (P->posx - initx);
                                    if (dx > 140 || dx < -140)
                                        std::cout<<"BBBBBBBBB:"<<dx<<std::endl;
                                }
                                else if (pair2.first >= TorusDim.first / (int)Physics_Engine.Distance)
                                {
                                    dx = TorusDim.first - (P->posx - initx);
                                    if (dx > 140 || dx < -140)
                                        std::cout<<"AFDHUWDFHWUDW:"<<dx<<std::endl;
                                }
                                else
                                {
                                    dx = P->posx - initx;
                                    if (dx > 140 || dx < -140)
                                        std::cout<<"DX:"<<dx<<std::endl;
                                }
                                if (pair2.second < 0)
                                {
                                    dy = TorusDim.second - (inity - P->posy);
                                }
                                else if (pair2.second >= TorusDim.second / (int)Physics_Engine.Distance)
                                {
                                    dy = TorusDim.second - (P->posx - initx);
                                }
                                else
                                {
                                    dy = P->posy - inity;
                                }
                            }
                            else
                            {
                                dx = P->posx - initx;
                                dy = P->posy - inity;
                            }
                            float distance = sqrtf(dx * dx + dy * dy);
                            local_particles.emplace_back(P, Triangle(distance, dx, dy));
                            if (distance < Physics_Engine.DensityRadius) {
                                if (n->ID == P->ID) {
                                    local_density += 1 - distance / Physics_Engine.DensityRadius;
                                }
                                else
                                    local_density += 0.5F - distance * .5 / Physics_Engine.DensityRadius;
                            }
                        }

                    }
                }
            }

            //Get accelerations
            for (auto info : local_particles) {
				Particle* P = info.first;
                if (P!=n && info.second.distance < Physics_Engine.Distance)
                {
                    float acc = Physics_Engine.Get_Acc(info.second.distance, n->ID, P->ID);

                    float density_factor = 1.0f;
                    if (acc > 0) {
                        density_factor = 1.0f - std::fminf(std::fmaxf(0.0f, local_density - Physics_Engine.MaxDensity), 2);
                    }
                    float acctodratio = acc / info.second.distance;
                    accx += density_factor * acctodratio * info.second.dx;
                    accy += density_factor * acctodratio * info.second.dy;
                        
                }      
            }
            n->accx = accx;
            n->accy = accy;
        }
    }


    //update position and velocity for all particles
    for (int n = 0; n<particle_num; n++)
    {
        Physics_Engine.Apply_Physics(
            particles[n].posx, particles[n].posy,
            particles[n].velx, particles[n].vely,
            particles[n].accx, particles[n].accy,
            Torus, TorusDim.first, TorusDim.second);
    }
}

void Sim::Update_Properties() //TODO: This may not need to be a function if its only used in Restart_Simulation()
{
    //Clear P_Textures
    for (int n = 0; n < typenum; n++)
    {
        SDL_DestroyTexture(renderer.P_Textures[n]);
    }
    renderer.P_Textures = nullptr;

    Properties props;

    particle_dist = props.particle_dist;
    particle_num = props.particlenum;

    //Particle properties
    P_radius = props.P_radius;
    typenum = props.typenum;
    color = props.color;

    //Physics properties
    Physics_Engine.Repel_Force = props.Repel_Force; //Max repel acceleration: Will be constant (Maybe)
    Physics_Engine.Max_Force = props.Max_Force; // Max acceleration
    Physics_Engine.Repel_Distance = props.Repel_Distance; //distance when acc is 0
    Physics_Engine.Distance = props.Distance; // distance when acc is 0
    Physics_Engine.Force_Radius = (Physics_Engine.Distance-Physics_Engine.Repel_Distance)/2;
    Physics_Engine.Friction = props.Friction;

    //World Properties
    Torus = props.Torus;
    TorusDim = props.TorusDim;

    //Create P_Textures
    renderer.P_Textures = CreateTexturesFromColors(typenum, P_radius, color, renderer.renderer);
}

void Sim::Restart_Simulation()
{
    //Update Sim properties
    particles.clear();
    Update_Properties();
    //Replace particles //TODO: I think this is duplicated code
    for (int n = 0; n < particle_dist.size(); n++) {
        for (int i = 0; i < particle_dist[n]; i++) {
            particles.emplace_back(
                rng.Get_Random(0, static_cast<float>(win.width)),
                rng.Get_Random(0, static_cast<float>(win.height)),
                n);
        }
    }
}

void Sim::Pull_Event()
{
    while (SDL_PollEvent(&win.event))
    {
        ImGui_ImplSDL3_ProcessEvent(&win.event);
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard && !io.WantCaptureMouse) {
            switch (win.event.type)
            {
            case SDL_EVENT_QUIT:
                SDL_Log("SDL3 event quit");
                run = false;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                settings.temppause = true;
                mouse_down = true;

                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                settings.temppause = false;
                mouse_down = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if (mouse_down) {
                    cam.posx += win.event.motion.xrel * cam.zoom;
                    cam.posy += win.event.motion.yrel * cam.zoom;
                }
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                if (win.event.wheel.y < 0)
                    cam.zoom *= 1.02;
                else
                    cam.zoom /= 1.02;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (win.event.key.key) {//TODO: Add a proper keyboard handler
            case SDLK_P:
                    if (!settings.pause)
                        settings.pause = true;
                    else settings.pause = false;
                    break;
            case SDLK_O:
                    if (!settings.Setting_display) {
                        settings.pause = true;
                        settings.Setting_display = true;
                        GUI = new GUIContext();
                    }
                    else {
                        settings.Setting_display = false;
                        settings.pause = false;
                        delete GUI;
                        GUI = nullptr;
                    }
                    break;
            case SDLK_R:
                    Restart_Simulation();
                    break;
            default:
                    break;
                }
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                win.width = win.event.window.data1;
                win.height = win.event.window.data2;
                break;
            default:
                break;
            }
        }
    }
}


Sim::~Sim()
{
    particles.clear();
}