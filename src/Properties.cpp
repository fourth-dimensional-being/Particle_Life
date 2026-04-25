#include "Properties.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
Properties::Properties() {
    Read();
}

void Properties::Read()
{
    //Cleat all vectors
    Max_Force.clear();
    color.clear();
    particle_dist.clear();

    //Parse Properties
    std::string line;
    std::ifstream Properties("Properties.txt");
    if (!Properties.is_open()) {
        std::cerr << "Unable to open file Properties.txt" << std::endl;
    }

    propertytype category;
    std::unordered_map<std::string, propertytype> map = { 
        {"PARTICLE_RADIUS", PARTICLE_RADIUS},
        {"NUMBER", NUMBER},
       {"TYPENUM", TYPENUM},
        {"COLOR", COLOR},
        {"FORCES", FORCES},
        {"REPEL_FORCE", REPEL_FORCE},
        {"REPEL_DISTANCE", REPEL_DISTANCE},
        {"FORCE_DISTANCE", FORCE_DISTANCE},
        {"FRICTION", FRICTION},
        {"TORUS", TORUS}
    };
    int linenum = 0;
    while (std::getline(Properties, line)) {
        // Output the text from the file
        if (map.contains(line))
        {
            category = map[line];
            linenum = 0;
        }
        else
        {
            switch (category)
            {
                case PARTICLE_RADIUS:
                P_radius = atof(line.c_str());
                break;
                case NUMBER:
                particle_dist.push_back(atof(line.c_str()));
                break;
                case TYPENUM:
                typenum = atof(line.c_str());
                break;
                case COLOR:
                color.push_back(atof(line.c_str())); 
                break;
                case FORCES:
                if (linenum%typenum == 0)
                {
                    Max_Force.emplace_back();
                }
                Max_Force[linenum/typenum].push_back(atof(line.c_str()));
                case REPEL_FORCE:
                Repel_Force = atof(line.c_str());
                break;
                case REPEL_DISTANCE:
                Repel_Distance = atof(line.c_str());
                break;
                case FORCE_DISTANCE:
                Distance = atof(line.c_str());
                case FRICTION:
                Friction = atof(line.c_str());
                break;
                case TORUS:
                    switch(linenum)
                    {
                        case 0:
                        Torus = atoi(line.c_str());
                        break;
                        case 1:
                        TorusDim.first = atof(line.c_str());
                        break;
                        case 2:
                        TorusDim.second = atof(line.c_str());
                        break;
                        default:
                        break;
                    }
            }
            linenum++;
        }
        particlenum = 0;
    }
    for (int n : particle_dist)
        particlenum += n;
    Properties.close();
}

void Properties::Write()
{
    std::ofstream Properties("Properties.txt");
    if (!Properties.is_open())
    {
        std::cerr<<"Could not find properties.txt\n";
    }
    Properties << "PARTICLE_RADIUS\n";
    Properties << P_radius << std::endl;

    Properties << "NUMBER\n";
    for (int n : particle_dist)
        Properties << n << std::endl;

    Properties << "TYPENUM\n";
    Properties << particle_dist.size() << std::endl;

    Properties << "COLOR\n";
    for (int n = 0; n < particle_dist.size(); n++)
        Properties << color[n] << std::endl;

    Properties << "FORCES\n";
    for (int init = 0; init < particle_dist.size(); init++) {
        for (int target = 0; target < particle_dist.size(); target++) {
            Properties << Max_Force[init][target] << std::endl;
        }
    }

    Properties << "REPEL_FORCE\n";
    Properties << Repel_Force<<std::endl;

    Properties << "REPEL_DISTANCE\n";
    Properties << Repel_Distance<<std::endl;

    Properties << "FORCE_DISTANCE\n";
    Properties << Distance<<std::endl;

    Properties << "FRICTION\n";
    Properties << Friction<<std::endl;

    Properties <<"TORUS\n";
    Properties <<Torus<<std::endl;
    Properties <<TorusDim.first<<std::endl;
    Properties <<TorusDim.second<<std::endl;

}