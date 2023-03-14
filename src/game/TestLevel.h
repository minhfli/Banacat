#pragma once
#include <string>
/*
    test loading file from ldtk file
*/

namespace sk_game {
    namespace test_level {
        const std::string level_path = "Assets/tilemap/test/level_0.ldtkl";
        const std::string tileset_path = "Assets/tilemap/Cavernas_by_Adam_Saltsman.png";
        void LoadLevel();
        void UnLoadLevel();
        void Draw();
    }
}