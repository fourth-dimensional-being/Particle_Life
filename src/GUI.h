#ifndef PARTICLE_LIFE_GUI_H
#define PARTICLE_LIFE_GUI_H
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <unordered_map>
#include "Structures.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "Properties.h"

//TODO: Put all layout constants here when cleaning the code

struct TorusBuffer
{
    bool enabled;
    std::vector<char> WidthData;
    std::vector<char> HeightData;
};

enum GuiCallBack
{
    SAVE,
    CANCEL,
    NONE
};

struct GUIContext
{
    Properties Propcopy;
    //Initialize all buffers
    std::vector<std::vector<std::vector<char>>> GridTextBuff;
    std::vector<float*> ColBuff; //TODO: Make sureyour deleting this memory
    std::vector<std::vector<char>> MenuTextBuff;
    std::unordered_map<std::string, std::vector<char>> PropertyTextBuff;
    TorusBuffer TorusBuff;

    GUIContext();
    void UpdateGUI();
    ~GUIContext();
};
//Properties and other GUI init stuff
//TODO: you should probably put all this stuff into a function like "CreateUIContext or something like that maybe"
inline GUIContext* GUI = nullptr;


//TODO: CONVERT EVERYTHING INTO PERCENTAGES OF PARENT OBJECTS
//Layout Constants
const float MenuButtonsHeightRatio = .2; //The height of the UI divided by the height of the Menu_bottons
const float SaveButtonsHeightRatio = .3; //The height of the Properties plus Save Buttons divided by the height of the save buttons
const float GridCircleDiameterRatio = .8; //The ratio between the diameter of the cicle and the spacing of the grid
const float MenuCircleDiameterRatio = .6; //The ratio between the diameter of the menu circle and the vertical spacing of the menu rectangle
const float MenuButtonSymbolRatio = .8; //The ratio of the size of the smbols within the MenuButtons to the size of the MenuButtons

//Children layout
const Rect UI(250, 250, 500, 500);
const Rect Grid(UI.x, UI.y, UI.w/2, UI.h/2);
const Rect Menu(UI.x + UI.w/2, UI.y, UI.w/2, UI.h*(1-MenuButtonsHeightRatio));
const Rect Menu_Buttons(Menu.x, Menu.y + Menu.h - MenuButtonsHeightRatio, Menu.w, UI.h * MenuButtonsHeightRatio);
const Rect properties(UI.x, UI.y + UI.h/2, UI.w/2, UI.h*(1-SaveButtonsHeightRatio)/2);
const Rect Save_Buttons(UI.x, properties.y + properties.h, properties.w, UI.h*SaveButtonsHeightRatio/2);

//Constants
constexpr int GridTextSize = 4;
const ImVec2 P_container(100, 50); //The dimensions of the Menu Rectangles
const float MenuSpacing = 50.0; //The distance between menu rectangles
const float MenuRadius = P_container.y * MenuCircleDiameterRatio/2; //The radius of the menu circles
const float circle_padding = P_container.y/2 - MenuRadius; // The distance between the edge of the circles and the edge of their containment rectangles
const float GridRadius = 50; // The radius of the grid circles
const ImVec2 MenuPadding(20, 20); //The padding on the menu.
const float GridSpacing = 50.0; //The width of grid squares
const float GridFont = 20.0; //The font size in the grid
const float MenuButtonSize = 50;
const ImVec2 MenuButtonPadding(20,20);
const float PropertyFontSize = 20.0;
const float PropertyTextSpacing = 20;
const ImVec2 SaveButtonPadding(50, 20);
const ImVec2 SaveButtonSize(50, Save_Buttons.h - 2*SaveButtonPadding.y);
const float GridCircleRadius = GridCircleDiameterRatio*MenuSpacing/2;
const float ColorTextSize = 15;
const float ColorPadding =  MenuRadius - ColorTextSize/2;
const int MenuTextSize = 5;
const float MenuFontSize = 15.0;
const float PropertyWidgetSpacing = 10.0;
constexpr float SymbolThickness = 4;
const float MenuSymbolPadding = MenuButtonSize*(1-MenuButtonSymbolRatio)/2;

GuiCallBack Create_UIDisplay(); //TODO: You can get rid of types in all these functions. Just use the size of the color array
void Create_Grid(); //To Be called within Create_UIDisplay
void Create_Menu(); //To Be called within Create_UIDisplay
void Create_Properties(); //To Be called within Create_UIDisplay
void Create_Torus();
void Create_PropertyText(const std::string &name, std::vector<char> &buff);
GuiCallBack Create_SaveButtons(); //To be called within Create_UIDisplay

std::vector<char> Create_PropertyBuffer(float input, int PropertyTextBuffSize);
void RenderUIDisplay();


#endif //PARTICLE_LIFE_GUI_H