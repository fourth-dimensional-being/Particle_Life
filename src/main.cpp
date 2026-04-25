#include <iostream>
#include "Sim.h"

int main() {
    //TODO: USE VECTOR<particle> instead of a list!!!!!!!!!!!!!!!!!
    
    Sim simulation;

    
    Uint64 framestart; //TODO: Make all Framerate stuff in SIM.
    Uint64 frametime;
    while (simulation.run) // TODO: Figure out how to make run in sim
    {
        framestart = SDL_GetTicks();
        simulation.Pull_Event();

        if (!simulation.settings.pause) {
            if (!simulation.settings.temppause) {
                simulation.Move_Step();
            }
        }
        
        
        SDL_RenderClear(simulation.renderer.renderer); //TODO: Research this
        simulation.renderer.RenderSim(
        simulation.Get_Coords()[0], simulation.Get_Coords()[1],
        simulation.Get_Types(),
        static_cast<float>(simulation.win.width), static_cast<float>(simulation.win.height),
        (float)P_radius,
        simulation.cam.posx, simulation.cam.posy, simulation.cam.zoom);


        if (simulation.settings.Setting_display) {
            GuiCallBack output = Create_UIDisplay(); //TODO: Color maybe should be part of sim
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), simulation.renderer.renderer);
            //SDL_RenderPresent(simulation.renderer.renderer);
            if (output != NONE)
            {
                //TODO: This is duplicated code so it should probably become a function or redesigned
                simulation.settings.Setting_display = false;
                simulation.settings.pause = false;
                delete GUI;
                GUI = nullptr;
                ImGuiIO& io = ImGui::GetIO();
                io.WantCaptureMouse = false;
                io.WantCaptureKeyboard = false;
                if (output == SAVE)
                    simulation.Restart_Simulation();
            }
        }
        SDL_RenderPresent(simulation.renderer.renderer);
        frametime = SDL_GetTicks()-framestart;
        if (Frame_Delay>frametime) {
            SDL_Delay(Frame_Delay-frametime);
        }
    }
    
    return 0;
}