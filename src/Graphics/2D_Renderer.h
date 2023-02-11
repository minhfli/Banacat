#pragma once

#include <GLM/glm.hpp>

#include "Vertex2D.h"

//? batch renderer 
class Renderer2D {

    static void Init();
    static void ShutDown();

    static void BeginBatch();
    static void EndBatch();
    static void Flush();        //? draw all 


};


