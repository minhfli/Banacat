#pragma once

#include <sk_engine/Graphics/Graphics.h>
#include <string>
#include <iostream>

namespace {
    void LoadAllTexture() {
        //? this game use thread to load between scene and level
        //? opengl cant work on multiple thead 
        //? -> load all texture before game start
        sk_graphic::Texture2D tex;
        tex.Load("Assets/BackGrounds/sky.png");
        tex.Load("Assets/Entity/Spring/spring.png");
        tex.Load("Assets/Entity/Dash_crystal/dash_crystal.png");
        tex.Load("Assets/Entity/Player/bananacat.png");
        tex.Load("Assets/Entity/Player/death.png");
        tex.Load("Assets/TileSet.png");
    }
}
void GAME_SETUP() {
    LoadAllTexture();
}