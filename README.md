# Particle_Life

A simple C++ project simulating particle life

# To use

Press P to pause
Press O to open settings
Press R to restart the simulation

# Build

This project requires CMake and is intented to be built with Ninja or Visual studio.
This project requires SDL3 and Dear Imgui. If you are on linux or windows, you shouldnt need to install these as they are included in this repository.

In the project directory, run
```bash
cmake -B build -S . -G "Ninja"
cmake --build build
```

Enjoy!