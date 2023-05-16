# BanKat Game project

how to compile and run: 

    open cmd
    make 

control:
    debug:
        keypad 7: turn debug(on/off)
        keypad 4,6: camera zoom(for debug only)
        L : slow time(for debug only)
        R : kill player(for debug only)

    player control:
        wasd: move
        space: jump
        j: wall grab
        k: dash

screen shots:

![feature-image](./Screenshot/main_menu.png)

![feature-image](./Screenshot/gameplay1.png)
    
![feature-image](./Screenshot/gameplay2.png)
    
![feature-image](./Screenshot/gameplay3.png)
    
![feature-image](./Screenshot/gameplay4.png)


sk engine is a simple 2d game engine using sdl and opengl written my Mr. MinhFli(me)

Third party library used:

    SDL2:               for window and opengl
    GLAD:               for opengl
    STB_IMAGE:          load .png files
    NLOHMANN_JSON:      load .json files
    GLM:                math library

sk_engine's modules:

    graphics:   

        -simple opengl 2d renderer, can draw textured quad and line
    
    physic2d:   

        -2d physic implementation, based on celeste and towerdall physic: https://www.mattmakesgames.com/articles/celeste_and_towerfall_physics/index.html, for narrow phase collision detection (src/sk_engine/Physics/AABB_World_resolve.cpp)

        -Quad tree for broad phase collision detection (src/sk_engine/Physics/AABB_QuadTree.cpp)

    window:

        -simple sdl wrapper for windowing
    
    audio:
    
        -simple sdl_mixxer wrapper for audio
    
    common: 
    
        -timer
        -readfile
        -handle error
