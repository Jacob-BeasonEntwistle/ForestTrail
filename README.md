# Forest Trail

*Developed by Jacob Beason Entwistle as part of a Game Engine Architecture module.*

## Overview
Forest Trail is a custom-built 3D scene rendered using a C++ OpenGL-based engine. The environment features a procedurally generated forest designed around exploration, with dynamic points of interest distributed throughout the world. Each run produces a slightly different layout through randomised object placement.

A lightweight GUI system assists navigation and displays real-time debug information, alongside a custom cursor indicating camera orientation.

This is a modular real-time 3D rendering engine built in C++ using OpenGL and SDL2. The engine supports procedural world generation, multiple rendering techniques, and independent subsystems for rendering, camera control, UI, and visual effects.

## Engine Systems
### Rendering system
- OpenGL-based model rendering pipeline
- Asset loading via Assimp
- Textured meshes with UV mapping (stone, wood, terrain materials)
- Static and dynamic models (e.g. moving hedgehog, rotating statue)
- Instanced rendering for trees and rocks
- Skybox environment rendering
- Real-time lighting system

### Camera system
- First-person camera with full mouse-look controls
- Third-person camera with target tracking
- Runtime switching between camera modes

### World/Scene system
- Procedurally generated forest environment
- Terrain-based scene layout
- Distributed points of interest (POIs)

### UI/HUD system
- Real-time FPS counter and engine statistics overlay
- Debug HUD (camera position, object counts, geometry stats, etc.)
- Minimap for spatial awareness

### Effects system
- Particle system (magic orb effect)
- Real-time visual effects integrated into rendering pipeline


## Future Features
- Shadow mapping for dynamic lighting
- Post-processing pipeline (bloom, tone mapping)
- Normal/bump mapping for terrain detail enhancement
- Reflections for water and reflective surfaces

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
Engine performance was evaluated across iterative development stages, with profiling focused on core loop execution (init, update, draw).

Key optimisations included:
- Reduced draw overhead through render-path optimisation
- Improved debug UI efficiency via cached calculations
- Updated nearest-object logic to reduce per-frame cost
- Initial groundwork for batching particle rendering

### Key Results
- Initial scene: ~61 FPS
- After UI optimisations: stable ~62 FPS
- After particle system addition: draw call became primary bottleneck (~28 ms average)

### Insight
The rendering pipeline is primarily GPU-bound during complex scenes, with particle rendering identified as the main performance constraint.