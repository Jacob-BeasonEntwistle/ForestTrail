# Forest Trail

Created by Jacob Beason Entwistle for Game Engine Architecture (CMP5363)

## Overview
The game scene will be a forest that encourages exploration and presents the player with points of interest (POI's) throughout the scene.

## Current Features
### 40%
- FPS camera
- Loading 3D models from file
- Five static models (rocks, fences, signposts, trees, crates)
- Simple terrain (flat plane)
- Movement
- Textures + UVs (stone, wood, grass/dirt ground)
- Skybox/Skydome

### 50-59%
- Third person camera (+ switching between povs)
- Dynamic models (rotating statue and hedgehog)
- FPS text (UI)

### 60-69%
- Instancing (trees, rocks)
- HUD (showing current camera mode, FPS, camera/player position, camera/player rotation, objects loaded)

## Future Features
### 50-59% (Optional)
- Billboard objects (distant trees)

### 60-69% (Optional)
- Minimap

### 70%+
- Lighting + shadows
- Special effects (particles - fire, leaves falling, embers, magic glow)
(Optional: Post-processing - bloom, bump maps, reflections in water/surfaces)

## Target Platform
- Windows

## Dependencies
- SDL2
- SDL2_image
- glm
- glew
- assimp

## How to Play
- WASD to move around
- Mouse to look around

## Code Profiling
05/03/2026 - 5 models, 1 dynamic model, flat terrain, camera movement
- Runtime performance: ~41 FPS (home machine)
- Game Engine Initialisation: ~745 average*

*Measured across five runs: 1292ms, 688ms, 582ms, 592ms, 571ms

09/02/2026 - empty scene with flat terrain
- Runtime performance: ~41 FPS (home machine)
- Game Engine Initialisation: ~432ms average*

*Measured across five runs: 405ms, 476ms, 449ms, 436ms, 398ms

## TODO: Add the following details and update regularly

1. [DONE] Name of your game scene
2. [DONE] Description/design
3. [DONE] Key features marrying them up to the assignment criteria, e.g. what features you will implement to achieve 50-59%, 60-69% and so on.
4. [DONE] Target platform
5. [DONE] Dependencies - libraries used such as SDL2, SDL2_image, etc.
6. [DONE] Playing the game - how to run the game and key bindings
7. [DONE] Code profiling results on Windows and Linux
8. Optimisations applied
9. References to sources used.  This must include graphics resources used such as models and textures, and code resources (websites, book, AI).
