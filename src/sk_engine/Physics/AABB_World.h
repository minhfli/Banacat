#pragma once

#include "Collider.h"
#include "AABB_QuadTree.h"

#include <vector>
/*
    currently only support rect
*/
namespace sk_physic2d {

    /// @brief physic world is a class, allow multiple physic world at the same time
    class AABB_World {
        private:

        /// @brief list of physic body
        std::vector <Body> m_Body = std::vector<Body>();
        int body_counts = 0;
        /// @brief stack of avaiable index for createing new physic body
        std::vector <int> avaiable_mbody_index = std::vector<int>();

        QuadTree quad_tree = QuadTree();

        public:
        inline int BodyCount()const { return body_counts; }

        bool debug_draw = true;

        void Init();

        /// @brief create new physic body and return its reference and index(optional)
        Body* Create_Body(const Body_Def& def, int* index = nullptr);
        /// @brief get the refference physic body at index  
        Body* Get_Body(const int index);
        /// @brief remove physic body
        void Remove_Body(const int index);
        /// @brief remove physic body using pointer (you should not use this, it may result in undefined behaviors) 
        void Remove_Body(Body* body);

        void GetQuadTreeDebug() { quad_tree.GetInfo(); };
        std::vector<int> Query(const rect& rect) { return quad_tree.Query(rect); };
        /// @brief remove all physic body from world

        void Update();
        void Draw();
        void Clear();
    };
}