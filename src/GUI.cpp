#include "GUI.h"
#include "imgui_internal.h"
#include <algorithm>
#include <charconv>
#include <string>

std::vector<char> Create_PropertyBuffer(float input, int PropertyTextBuffSize)
{
    std::vector<char> buff(PropertyTextBuffSize);
    std::snprintf(buff.data(), PropertyTextBuffSize, "%f", input);
    return buff;
}
std::vector<char> Create_PropertyBuffer(int input, int PropertyTextBuffSize)
{
    std::vector<char> buff(PropertyTextBuffSize);
    std::snprintf(buff.data(), PropertyTextBuffSize, "%d", input);
    return buff;
}


GUIContext::GUIContext () : Propcopy()
{
    //Make GridTextBuff
    for (int y = 0; y < Propcopy.typenum; y++)
    {
        GridTextBuff.emplace_back();
        for (int x = 0; x < Propcopy.typenum; x++)
        {
            //convert the float value into std::vector<char> //TODO: add an axception case throw error thing when the float is abouve 4 characters
            std::vector<char> buff(GridTextSize+1);
            std::string value = std::to_string(Propcopy.Max_Force[y][x]);
            for (int n = 0; n < GridTextSize; n++)
            {
                buff[n] = value[n];
            }
            GridTextBuff[y].push_back(buff);
        }
    }

    //Make ColBuff
    for (int n = 0; n < Propcopy.typenum; n++)
    {
        auto colors = new float[3];
        colors[0] = (float)(Uint8(Propcopy.color[n] >> 24u))/255.0;
        colors[1] = (float)(Uint8(Propcopy.color[n] >> 16u))/255.0;
        colors[2] = (float)(Uint8(Propcopy.color[n] >> 8u))/255.0;
        ColBuff.push_back(colors);
    }

    //Make MenuTextBuff
    for (int n = 0; n < Propcopy.typenum; n++)
    {
        MenuTextBuff.emplace_back(MenuTextSize+1);
        std::string value2 = std::to_string(Propcopy.particle_dist[n]);
        for (int i = 0; i < std::min(MenuTextSize, (int)value2.size()); i++)
        {
            MenuTextBuff[n][i] = value2[i];
        }
    }

    //Make PropertyTextBuff
    PropertyTextBuff["Particle_Radius"] = Create_PropertyBuffer(Propcopy.P_radius, 5);
    PropertyTextBuff["Repel_Force"] = Create_PropertyBuffer(Propcopy.Repel_Force, 5);
    PropertyTextBuff["Repel_Distance"] = Create_PropertyBuffer(Propcopy.Repel_Distance, 5);
    PropertyTextBuff["Force_Distance"] = Create_PropertyBuffer(Propcopy.Distance, 5);
    PropertyTextBuff["Friction"] = Create_PropertyBuffer(Propcopy.Friction, 5);

    //Make TorusBuff
    TorusBuff.enabled = Propcopy.Torus;
    TorusBuff.WidthData = Create_PropertyBuffer(Propcopy.TorusDim.first, 6);
    TorusBuff.HeightData = Create_PropertyBuffer(Propcopy.TorusDim.second, 6);
}

void GUIContext::UpdateGUI() //Load all buffer data into properties
{
    //Clear all vectors
    Propcopy.Max_Force.clear();
    Propcopy.color.clear();
    Propcopy.particle_dist.clear();

    //Convert the Gridbuffer into floats
    for (int y = 0; y < Propcopy.typenum; y++)
    {
        Propcopy.Max_Force.emplace_back();
        for (int x = 0; x < Propcopy.typenum; x++)
        {
            Propcopy.Max_Force[y].push_back(std::atof(GridTextBuff[y][x].data()));
            if (std::all_of(GridTextBuff[y][x].begin(), GridTextBuff[y][x].end(), [](char c){return c == '\0';})) {
                Propcopy.Max_Force[y].push_back(0);
            }
        }
    }

    //Convert the ColBuff into Uint32 in RGBA
    for (int n = 0; n < Propcopy.typenum; n++)
    {
        Propcopy.color.push_back(Uint32(Uint8(255*ColBuff[n][0]) << 24u | Uint8(255*ColBuff[n][1]) << 16u | Uint8(255*ColBuff[n][2]) << 8u | 0xFF));
    }

    //Convert The Menutextbuffer into int
    for (int n = 0; n < Propcopy.typenum; n++)
    {
        Propcopy.particle_dist.push_back(std::atoi(MenuTextBuff[n].data()));
    }

    //Convert The PropertyBuffer into float and int. //TODO: Make better
    Propcopy.P_radius = std::atoi(PropertyTextBuff["Particle_Radius"].data());
    Propcopy.Repel_Force = std::atof(PropertyTextBuff["Repel_Force"].data());
    Propcopy.Repel_Distance = std::atof(PropertyTextBuff["Repel_Distance"].data());
    Propcopy.Distance = std::atof(PropertyTextBuff["Force_Distance"].data());
    Propcopy.Friction = std::atof(PropertyTextBuff["Friction"].data());

    //Convert the TorusBuffer
    Propcopy.Torus = TorusBuff.enabled;
    Propcopy.TorusDim.first = std::atoi(TorusBuff.WidthData.data());
    Propcopy.TorusDim.second = std::atoi(TorusBuff.HeightData.data());
}

GUIContext::~GUIContext ()
{
    //Destroy buffers
    for (float* n : ColBuff)
    {
        delete[] n;
    }
}


GuiCallBack Create_UIDisplay() {
    bool Display = 1; //TODO: Fix this

    if (GUI == nullptr)
    {
        std::cerr<<"GUI not initialized"<<std::endl;
    }


    //Init ImGui Frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    //Set style

    ImGui::SetNextWindowPos(ImVec2(UI.x, UI.y));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1, 0.1, 0.1, 1.0));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1, 0.1, 0.1, 1.0));


    ImGui::Begin("##UIDisplay", &Display, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    Create_Menu();
    Create_Grid();
    Create_Properties();
    GuiCallBack output = Create_SaveButtons();

    //Submit draw data
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::Render();
    return output;
};


void Create_Grid() {
    float font = 20;
    float widgetpadding = 10;
    ImGui::SetCursorScreenPos(ImVec2(Grid.x, Grid.y));
    ImGui::BeginChild("##Grid", ImVec2(Grid.w, Grid.h), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImDrawList* Draw_List = ImGui::GetCurrentWindow()->DrawList;
    ImVec2 scroll = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());
    ImVec2 widgetdim = {(float)(GUI->Propcopy.typenum + 1)*GridSpacing+2*widgetpadding, (float)(GUI->Propcopy.typenum + 1)*GridSpacing+2*widgetpadding};
    ImVec2 widgetpos = {Grid.x - scroll.x, Grid.y - scroll.y};
    if (widgetdim.x < Grid.w) {
        widgetpos.x = Grid.x - scroll.x + (Grid.w - widgetdim.x)/2;
    }
    if (widgetdim.y < Grid.h) {
        widgetpos.y = Grid.y - scroll.y + (Grid.h - widgetdim.y)/2;
    }
    ImGui::Dummy(widgetdim);
    //Making Text
    for (int y = 0; y < GUI->Propcopy.typenum; y++) {
        for (int x = 0; x < GUI->Propcopy.typenum; x++) {
            ImGui::SetCursorScreenPos(ImVec2(widgetpos.x + ((float)x+1.0f)*GridSpacing + widgetpadding, widgetpos.y + ((float)y+1.0f)*GridSpacing + widgetpadding));
            ImGui::PushID(y*GUI->Propcopy.typenum + x);
            ImGui::PushItemWidth(GridSpacing);
            ImGui::PushFont(ImGui::GetFont(), font);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().ItemSpacing.x, (GridSpacing - font)/2));
            ImGui::InputText("##", GUI->GridTextBuff[y][x].data(), GridTextSize+1);
            ImGui::PopStyleVar();
            ImGui::PopFont();
            ImGui::PopID();
        }
    }
    //Drawing grid
    for (int n = 1; n < GUI->Propcopy.typenum + 2; n++) {
        Draw_List->AddLine(ImVec2(widgetpos.x + widgetpadding + (float)n * GridSpacing, widgetpos.y + widgetpadding),
                            ImVec2(widgetpos.x + widgetpadding + (float)n * GridSpacing, widgetpos.y + widgetdim.y - widgetpadding), 0xffffffff, 1.0F);
        Draw_List->AddLine(ImVec2(widgetpos.x + widgetpadding, widgetpos.y + n * GridSpacing + widgetpadding),
                            ImVec2(widgetpos.x + widgetdim.x - widgetpadding, widgetpos.y + n * GridSpacing + widgetpadding), 0xffffffff, 1.0F);

        if (n != GUI->Propcopy.typenum + 1)
        {
            Draw_List->AddCircleFilled(ImVec2(widgetpos.x + widgetpadding + GridSpacing/2,
                                        widgetpos.y + widgetpadding + (float)n * GridSpacing + GridSpacing/2),
                                        GridCircleRadius,
                                        ImU32(0xFF << 24u | Uint8(255*GUI->ColBuff[n - 1][2]) << 16u | Uint8(255*GUI->ColBuff[n - 1][1]) << 8u | Uint8(255*GUI->ColBuff[n - 1][0])));
            Draw_List->AddCircleFilled(ImVec2(widgetpos.x + widgetpadding + (float)n * GridSpacing + GridSpacing/2,
                                        widgetpos.y + widgetpadding + GridSpacing/2),
                                        GridCircleRadius,
                                    ImU32(0xFF << 24u | Uint8(255*GUI->ColBuff[n - 1][2]) << 16u | Uint8(255*GUI->ColBuff[n - 1][1]) << 8u | Uint8(255*GUI->ColBuff[n - 1][0])));
        }
    }


    ImGui::EndChild();
}

void Create_Menu()
{
    ImGui::SetCursorScreenPos(ImVec2(Menu.x, Menu.y));
    ImGui::BeginChild("##Menu", ImVec2(Menu.w, Menu.h), false, false);
    ImDrawList* Draw_List = ImGui::GetCurrentWindow()->DrawList;
    ImVec2 Scroll = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());
    Rect Menu_Rect(Menu.x - Scroll.x,Menu.y - Scroll.y ,ImGui::GetContentRegionAvail().x, 2*MenuPadding.y+GUI->Propcopy.typenum*(MenuSpacing - 1) + (GUI->Propcopy.typenum - 1)*P_container.y);
    ImGui::Dummy(ImVec2(Menu_Rect.w, Menu_Rect.h));
    //Create menu rectangles
    for (int n = 0; n < GUI->Propcopy.typenum; n++)
    {
        ImVec2 top_left(Menu_Rect.x + (Menu_Rect.w - P_container.x)/2,Menu_Rect.y + MenuPadding.y+n*(MenuSpacing + P_container.y));
        ImVec2 Bottom_Right(Menu_Rect.x + (Menu_Rect.w + P_container.x)/2, Menu_Rect.y + MenuPadding.y+n*(MenuSpacing + P_container.y) + P_container.y);
        Draw_List -> AddRect(top_left, Bottom_Right, 0xffffffff);
        ImGui::SetCursorScreenPos(ImVec2(top_left.x + circle_padding, top_left.y + circle_padding));
        ImGui::PushID(n);
        ImGui::PushFont(ImGui::GetFont(), ColorTextSize);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ColorPadding, ColorPadding));
        ImGui::ColorEdit3("##Coloredit", GUI->ColBuff[n], ImGuiColorEditFlags_NoInputs);
        ImGui::PopStyleVar();
        ImGui::PopFont();
        ImGui::PopID();
        Draw_List = ImGui::GetCurrentWindow()->DrawList;
        Draw_List -> AddRectFilled(ImVec2(top_left.x + circle_padding, top_left.y + circle_padding), ImVec2(top_left.x + circle_padding+2*MenuRadius, top_left.y + circle_padding + 2*MenuRadius), 0xFF1A1A1A); // (.1, .1, .1, 1) RGBA
        Draw_List -> AddCircleFilled( ImVec2(top_left.x + MenuRadius + circle_padding, top_left.y + P_container.y/2), MenuRadius, ImU32(0xFF << 24u | Uint8(255*GUI->ColBuff[n][2]) << 16u | Uint8(255*GUI->ColBuff[n][1]) << 8u | Uint8(255*GUI->ColBuff[n][0])));
        ImGui::SetCursorScreenPos(ImVec2(top_left.x + 2*(MenuRadius + circle_padding), top_left.y + circle_padding));
        ImGui::PushID(n);
        ImGui::PushItemWidth(P_container.x - 2*(MenuRadius+circle_padding) - 1); //TODO: This 1 represents the boarder width of the menu object, you should make that a variable
        ImGui::PushFont(ImGui::GetFont(), MenuFontSize);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().ItemSpacing.x, MenuRadius - MenuFontSize/2));
        ImGui::InputText("##Textedit", GUI->MenuTextBuff[n].data(), MenuTextSize + 1);
        ImGui::PopStyleVar();
        ImGui::PopFont();
        ImGui::PopItemWidth();
        ImGui::PopID();
    }

    ImGui::EndChild();


    //Create Buttons

    ImGui::SetCursorScreenPos(ImVec2(Menu_Buttons.x, Menu_Buttons.y));
    ImGui::BeginChild("##Menu_Buttons", ImVec2(Menu_Buttons.w, Menu_Buttons.h), false, false);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 255, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.078 , .38, .078, 255));
    ImGui::SetCursorScreenPos(ImVec2(Menu_Buttons.x + MenuButtonPadding.x, Menu_Buttons.y + MenuButtonPadding.y));
    if (ImGui::Button("##Add", ImVec2(MenuButtonSize, MenuButtonSize)))
    {
        GUI->Propcopy.typenum++;
        for (int n = 0; n < GUI->Propcopy.typenum - 1; n++)
        {
            GUI->Propcopy.Max_Force[n].push_back(0);
            GUI->GridTextBuff[n].emplace_back(GridTextSize+1);
        }
        GUI->Propcopy.Max_Force.emplace_back(GUI->Propcopy.typenum,0);
        GUI->GridTextBuff.emplace_back(GUI->Propcopy.typenum,std::vector<char>(GridTextSize+1));
        GUI->ColBuff.push_back(new float[3]{1,1,1}); //TODO: This is a magic number, maybe make a default_color variable
        GUI->Propcopy.particle_dist.push_back(100); //TODO: Thats a magic number, MAKE DEFAULT VARIABLES
        GUI->Propcopy.particlenum+=100;
        std::string string("100" ,MenuTextSize + 1);
        GUI->MenuTextBuff.emplace_back(string.begin(), string.end());
        GUI->Propcopy.color.push_back(0xFFFFFFFF);
    }
    ImGui::SetCursorScreenPos(ImVec2(Menu_Buttons.x + Menu_Buttons.w - MenuButtonPadding.x - MenuButtonSize, Menu_Buttons.y + MenuButtonPadding.y));
    if (ImGui::Button("##Remove", ImVec2(MenuButtonSize, MenuButtonSize)))
    {
        if (GUI->Propcopy.typenum > 1)
        {
            GUI->Propcopy.typenum--;
            for (int n = 0; n < GUI->Propcopy.typenum + 1; n++)
            {
                GUI->Propcopy.Max_Force[n].pop_back();
                GUI->GridTextBuff[n].pop_back();
            }
            GUI->Propcopy.Max_Force.pop_back();
            GUI->GridTextBuff.pop_back();
            GUI->MenuTextBuff.pop_back();
            GUI->Propcopy.particlenum-=GUI->Propcopy.particle_dist.back();
            GUI->Propcopy.particle_dist.pop_back(); //TODO: Thats a magic number
            GUI->Propcopy.color.pop_back();
        }
    }

    //Draw Symbols
    Draw_List = ImGui::GetCurrentWindow()->DrawList;

    //Plus
    Draw_List->AddLine(ImVec2(Menu_Buttons.x + MenuButtonPadding.x + MenuSymbolPadding, Menu_Buttons.y + MenuButtonPadding.y + MenuButtonSize/2),
                       ImVec2(Menu_Buttons.x + MenuButtonPadding.x + MenuButtonSize - MenuSymbolPadding, Menu_Buttons.y + MenuButtonPadding.y + MenuButtonSize/2), ImU32(0xFF000000), SymbolThickness);
    Draw_List->AddLine(ImVec2(Menu_Buttons.x + MenuButtonPadding.x + MenuButtonSize/2, Menu_Buttons.y + MenuButtonPadding.y + MenuSymbolPadding),
                       ImVec2(Menu_Buttons.x + MenuButtonPadding.x + MenuButtonSize/2, Menu_Buttons.y + MenuButtonPadding.y + MenuButtonSize - MenuSymbolPadding), ImU32(0xFF000000), SymbolThickness);
    //Minus
    Draw_List->AddLine(ImVec2(Menu_Buttons.x + Menu_Buttons.w - MenuButtonPadding.x - MenuButtonSize + MenuSymbolPadding, Menu_Buttons.y + MenuButtonPadding.y + MenuButtonSize/2),
                    ImVec2(Menu_Buttons.x + Menu_Buttons.w -  MenuButtonPadding.x - MenuSymbolPadding, Menu_Buttons.y + MenuButtonPadding.y + MenuButtonSize/2), ImU32(0xFF000000), SymbolThickness);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::EndChild();
}

void Create_Properties()
{
    ImGui::SetCursorScreenPos(ImVec2(properties.x, properties.y));
    ImGui::BeginChild("##Properties", ImVec2(properties.w, properties.h), false, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(ImGui::GetFont(), PropertyFontSize);

    for (auto &value : GUI->PropertyTextBuff) { //TODO: Make this a function;
        Create_PropertyText(value.first, value.second);
    }
    //Make Torus
    ImGui::Text("Torus World");
    ImGui::SameLine();
    ImGui::Checkbox("##TorusCheck", &GUI->TorusBuff.enabled);
    if (GUI->TorusBuff.enabled)
    {
        Create_Torus();
    }
    ImGui::PopFont();

    ImGui::EndChild();

}

GuiCallBack Create_SaveButtons()
{
    GuiCallBack output = NONE;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); //Make the button transparent
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.078 , .38, .078, 255));
    ImGui::SetCursorScreenPos(ImVec2(Save_Buttons.x, Save_Buttons.y));
    ImGui::BeginChild("##SaveButtons", ImVec2(Save_Buttons.w, Save_Buttons.h), false, false);
    ImGui::SetCursorScreenPos(ImVec2(Save_Buttons.x + SaveButtonPadding.x, Save_Buttons.y + SaveButtonPadding.y));
    if (ImGui::Button("Save", SaveButtonSize))
    {
        GUI->UpdateGUI();
        GUI->Propcopy.Write();
        output = SAVE;
    }
    ImGui::SetCursorScreenPos(ImVec2(Save_Buttons.x + Save_Buttons.w - SaveButtonPadding.x - SaveButtonSize.x, Save_Buttons.y + SaveButtonPadding.y));
    if (ImGui::Button("Cancel", SaveButtonSize))
    {
        output = CANCEL;
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::EndChild();
    return output;
}

void Create_Torus()
{
    Create_PropertyText("Width", GUI->TorusBuff.WidthData);
    Create_PropertyText("Height", GUI->TorusBuff.HeightData);
}


void Create_PropertyText(const std::string &name, std::vector<char> &buff)
{
    ImVec2 Length = ImGui::CalcTextSize(std::string(name+":").c_str());
    ImGui::Text(std::string(name+":").c_str());
    ImGui::SameLine(0, PropertyWidgetSpacing);
    ImGui::PushItemWidth(properties.w - Length.x - PropertyWidgetSpacing);
    ImGui::InputText(std::string("##"+name).c_str(), buff.data(), buff.size());
    ImGui::PopItemWidth();
}
