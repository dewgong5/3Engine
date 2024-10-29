# Introducing 3Engine (a Simple yet Powerful 3D Game Engine)

A 3D C++ Game Engine with ray-casting (shading of walls and floor), basic players controls, and collission detection!

## Features

- **📖Basic Rendering**: 3Engine can render walls, floors, and simulate depth with only ASCI characters and no graphics library!
- **🐻Ray/Light Casting**: 3Engine utilizes ray-casting techniques to detect walls and simulate player view, introduces shading of walls and floors.
- **🐲Player Controls**: 3Engine allows players to freely roam the map and explore a simple 3D layout using WASD controls, however players can't walk through walls and experience collission.

## Controls

- **W** - Move forward
- **S** - Move backward
- **A** - Rotate left
- **D** - Rotate right

## How to Run

1. **Requirements**: Windows OS and a compiler that supports C++11 or later.
2. **Compilation**: Compile the code using your preferred C++ compiler.
   ```bash
   g++ -o game simple_3d_game_engine.cpp -std=c++11

## Credits!
This project was inspired by [javidx9's Simple 3D Engine Video](https://www.youtube.com/watch?v=xW8skO7MFYw) and provides the base for the complicated math and ray-casting techniques.
