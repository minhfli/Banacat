#pragma once

#include "Collider.h"
#include "AABB_QuadTree.h"

#include <vector>
/*
    currently only support rect
*/
namespace sk_physic2d {

    class AABB_World {
        private:

        /// @brief list of physic body
        std::vector <Body> m_Body;
        int body_counts = 0;
        /// @brief stack of avaiable index for createing new physic body
        std::vector <int> avaiable_mbody_index;

        QuadTree quad_tree;

        public:
        bool debug_draw = true;

        void Init();

        Body* Create_Body(int* index = nullptr);
        void Remove_Body(int& index);
        void Remove_Body(Body* body);

        void Update();
        void Draw();
        void Clear();
    };
}