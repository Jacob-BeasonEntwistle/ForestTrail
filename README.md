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
- HUD (current camera mode, FPS, camera/player position, camera/player rotation, objects loaded, nearest object, num of vertices, num of triangles)

### 70%+
- Lighting

## Future Features
### 50-59% (Optional)
- Billboard objects (distant trees)

### 60-69% (Optional)
- Minimap

### 70%+
- Shadows
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
- freetype

## How to Play
- WASD to move around
- Mouse to look around
- TAB to toggle camera mode
- F1 to open debug stats

---

## Code Profiling
### <u>[08/04/2026]</u> - After optimising drawing UI stats
- **FPS**: ~42 FPS
- **ge.init()**: ~881.6ms
- **ge.update()**: ~0.192ms average
- **ge.draw()**: ~23.51ms average

|Function|Run 1|Run 2|Run 3|Run 4|Run 5|&nbsp;|Average|
|---|---|---|---|---|---|&nbsp;|---|
|ge.init()|814ms|889ms|823ms|969ms|913ms|&nbsp;|881.6ms|
|ge.update()|0.14ms|0.16ms|0.21ms|0.18ms|0.27ms|&nbsp;|0.192ms|
|ge.draw()|23.97ms|23.50ms|23.67ms|23.50ms|23.02ms|&nbsp;|23.51ms|

**Improvement:** 1.37ms

Optimised debug stats, rendering and performance profiling by caching entity/vertex counts, timing key functions, and updating nearest object calculations less frequently. Streamlined code with inline conditionals and efficient distance computations, reducing intialisation and frame update times noticeably.

### <u>[08/04/2026</u>] - 5 models, 1 dynamic model, instanced models, flat terrain, UI elements
- **FPS**: ~42 FPS
- **ge.init()**: ~1034ms average
- **ge.update()**: ~0.22ms average
- **ge.draw()**: ~24.88ms average

|Function|Run 1|Run 2|Run 3|Run 4|Run 5|&nbsp;|Average|
|---|---|---|---|---|---|&nbsp;|---|
|ge.init()|1998ms|718ms|793ms|842ms|821ms|&nbsp;|1034ms|
|ge.update()|0.25ms|0.13ms|0.30ms|0.20ms|0.23ms|&nbsp;|0.22ms|
|ge.draw()|24.62ms|24.76ms|27.86ms|23.54ms|23.66ms|&nbsp;|24.88ms|

Draw function is the bottleneck - limits performance. Requires some optimisation to reduce time taken for call.

### <u>[05/03/2026]</u> - 5 models, 1 dynamic model, flat terrain, camera movement
- Runtime performance: ~41 FPS (home machine)
- Game Engine Initialisation: ~745ms average*

*Measured across five runs:

|Function|Run 1|Run 2|Run 3|Run 4|Run 5|&nbsp;|Average|
|---|---|---|---|---|---|&nbsp;|---|
|ge.init()|1292ms|688ms|582ms|592ms|571ms|&nbsp;|745ms|

### <u>[09/02/2026]</u> - empty scene with flat terrain
- Runtime performance: ~41 FPS (home machine)
- Game Engine Initialisation: ~432ms average*

*Measured across five runs:

|Function|Run 1|Run 2|Run 3|Run 4|Run 5|&nbsp;|Average|
|---|---|---|---|---|---|&nbsp;|---|
|ge.init()|405ms|476ms|449ms|436ms|398ms|&nbsp;|432ms|

## TODO: Add the following details and update regularly

1. [DONE] Name of your game scene
2. [DONE] Description/design
3. [DONE] Key features marrying them up to the assignment criteria, e.g. what features you will implement to achieve 50-59%, 60-69% and so on.
4. [DONE] Target platform
5. [DONE] Dependencies - libraries used such as SDL2, SDL2_image, etc.
6. [DONE] Playing the game - how to run the game and key bindings
7. [DONE] Code profiling results on Windows and Linux
8. [DONE] Optimisations applied
9. References to sources used.  This must include graphics resources used such as models and textures, and code resources (websites, book, AI).
