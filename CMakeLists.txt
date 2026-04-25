cmake_minimum_required(VERSION 3.28)
project(Particle_Life)

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    set(CMAKE_GENERATOR "Visual Studio 17 2022")
else()
    set(CMAKE_GENERATOR "Ninja")
endif()

set(dependencydir win32)
if (UNIX) 
    set(dependencydir unix)
endif()
set(SDL3_dir dependencies/${dependencydir}/SDL3)
set(CMAKE_PREFIX_PATH ${SDL3_dir})


find_package(SDL3 CONFIG REQUIRED)

add_library(imgui
    dependencies/${dependencydir}/imgui/imgui.cpp
    dependencies/${dependencydir}/imgui/imgui_demo.cpp
    dependencies/${dependencydir}/imgui/imgui_draw.cpp
    dependencies/${dependencydir}/imgui/imgui_tables.cpp
    dependencies/${dependencydir}/imgui/imgui_widgets.cpp
    dependencies/${dependencydir}/imgui/backends/imgui_impl_sdl3.cpp
    dependencies/${dependencydir}/imgui/backends/imgui_impl_sdlrenderer3.cpp
)


target_include_directories(imgui PUBLIC
    dependencies/${dependencydir}/imgui
    dependencies/${dependencydir}/imgui/backends
)


add_executable(Particle_Life
        src/main.cpp
        src/Particle.cpp
        src/sim.cpp
        src/Renderer.cpp
        src/RNG.cpp
        src/Physics.cpp
        src/Window.cpp
        src/Camera.cpp
        src/Global.h
        src/GUI.cpp
        src/Structures.h
        src/Properties.h
        src/Properties.cpp
)


target_link_libraries(imgui PUBLIC SDL3::SDL3)
target_link_libraries(Particle_Life PRIVATE imgui)
