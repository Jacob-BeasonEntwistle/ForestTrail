# Forest Trail

Created by Jacob Beason Entwistle for Game Engine Architecture (CMP5363)

## Overview
The game scene is be a forest that encourages exploration and presents the player with points of interest (POI's) throughout the scene.

The forest layout is randomised each time the scene is run, scattering rocks and trees across the map. A GUI helps to guide the player and a custom cursor shows the player the direction they are facing. 

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
- Dynamic models (rotating statue and walking hedgehog)
- FPS text (UI)

### 60-69%
- Instancing (trees, rocks)
- HUD (current camera mode, FPS, camera/player position, camera/player rotation, objects loaded, nearest object, num of vertices, num of triangles)
- Minimap

### 70%+
- Lighting
- Special effects (particle system - magic glow around orb)

## Future Features
### 70%+
- Shadows
(Optional: Post-processing - bloom, bump maps, reflections in water/surfaces)

## Target Platform
- Windows

## Dependencies
- SDL2
- SDL2_image
- glm
- glew
- assimp
- freetype

## How to Play
- WASD to move around
- Mouse to look around
- Left Shift to sprint
- TAB to toggle camera mode
- F1 to open debug stats

---

## Code Profiling
### <u>[03/05/2026]</u> - After adding particle system
- **FPS**: ~41FPS (home machine)
- **ge.init()**: ~869.6ms average
- **ge.update()**: ~0.162ms average
- **ge.draw()**: ~28.84ms average

|Function	|Run 1	|Run 2	|Run 3	|Run 4	|Run 5	|Average|
|---		|---	|---	|---	|---	|---	|---	|
|ge.init()	|1529ms	|713ms	|677ms	|703ms	|726ms	|869.6ms|
|ge.update()|0.09ms	|0.15ms	|0.21ms	|0.17ms	|0.19ms	|0.162ms|
|ge.draw()	|27.21ms|28.51ms|28.23ms|29.56ms|30.97ms|28.84ms|

Performance could be improved by using batch rendering to draw the particles in the particle system.

Batch rendering the particles would reduce the number of draw calls and lower the CPU load

### <u>[09/04/2026]</u> - After optimising drawing UI stats
- **FPS**: ~42 FPS (home machine)
- **ge.init()**: ~881.6ms average
- **ge.update()**: ~0.192ms average
- **ge.draw()**: ~23.51ms average

|Function	|Run 1	|Run 2	|Run 3	|Run 4	|Run 5	|Average|
|---		|---	|---	|---	|---	|---	|---	|
|ge.init()	|814ms	|889ms	|823ms	|969ms	|913ms	|881.6ms|
|ge.update()|0.14ms	|0.16ms	|0.21ms	|0.18ms	|0.27ms	|0.192ms|
|ge.draw()	|23.97ms|23.50ms|23.67ms|23.50ms|23.02ms|23.51ms|

**Improvement:** 1.37ms

Optimised debug stats, rendering and performance profiling by caching entity/vertex counts, timing key functions, and updating nearest object calculations less frequently. Streamlined code with inline conditionals and efficient distance computations, reducing intialisation and frame update times noticeably.

### <u>[08/04/2026</u>] - 5 models, 1 dynamic model, instanced models, flat terrain, UI elements
- **FPS**: ~42 FPS (home machine)
- **ge.init()**: ~1034ms average
- **ge.update()**: ~0.22ms average
- **ge.draw()**: ~24.88ms average

|Function	|Run 1	|Run 2	|Run 3	|Run 4	|Run 5	|Average|
|---		|---	|---	|---	|---	|---	|---	|
|ge.init()	|1998ms	|718ms	|793ms	|842ms	|821ms	|1034ms	|
|ge.update()|0.25ms	|0.13ms	|0.30ms	|0.20ms	|0.23ms	|0.22ms	|
|ge.draw()	|24.62ms|24.76ms|27.86ms|23.54ms|23.66ms|24.88ms|

Draw function is the bottleneck - limits performance. Requires some optimisation to reduce time taken for call.

### <u>[05/03/2026]</u> - 5 models, 1 dynamic model, flat terrain, camera movement
- Runtime performance: ~41 FPS (home machine)
- Game Engine Initialisation: ~745ms average*

*Measured across five runs:

|Function	|Run 1	|Run 2	|Run 3	|Run 4	|Run 5	|Average|
|---		|---	|---	|---	|---	|---	|---	|
|ge.init()	|1292ms	|688ms	|582ms	|592ms	|571ms	|745ms	|

### <u>[09/02/2026]</u> - empty scene with flat terrain
- Runtime performance: ~41 FPS (home machine)
- Game Engine Initialisation: ~432ms average*

*Measured across five runs:

|Function	|Run 1	|Run 2	|Run 3	|Run 4	|Run 5	|Average|
|---		|---	|---	|---	|---	|---	|---	|
|ge.init()	|405ms	|476ms	|449ms	|436ms	|398ms	|432ms	|

## TODO: Add the following details and update regularly

1. [DONE] Name of your game scene
2. [DONE] Description/design
3. [DONE] Key features marrying them up to the assignment criteria, e.g. what features you will implement to achieve 50-59%, 60-69% and so on.
4. [DONE] Target platform
5. [DONE] Dependencies - libraries used such as SDL2, SDL2_image, etc.
6. [DONE] Playing the game - how to run the game and key bindings
7. [DONE] Code profiling results on Windows and Linux
8. [DONE] Optimisations applied
9. Port latest version to Linux
10. References to sources used.  This must include graphics resources used such as models and textures, and code resources (websites, book, AI).
