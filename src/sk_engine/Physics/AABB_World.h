#pragma once

#include "Collider.h"
#include "AABB_QuadTree.h"

#include <vector>
#include <deque>
/*
    currently only support rect
*/
namespace sk_physic2d {

    /// @brief physic world is a class, allow multiple physic world at the same time
    class AABB_World {
        private:

        /// @brief list of physic body
        std::deque <Body> m_Body = std::deque<Body>();
        int body_counts = 0;
        /// @brief stack of avaiable index for createing new physic body
        std::vector <int> avaiable_mbody_index = std::vector<int>();

        QuadTree quad_tree = QuadTree();

        public:
        inline int BodyCount()const { return body_counts; }

        bool enable_debug_draw = false;

        glm::vec4 world_bound = glm::vec4(-128, -128, 128, 128);

        //* hint funtions, call before Init to setup physic world 
        void Hint_WorldBound(glm::vec4 bound);
        void Hint_WorldBound(glm::vec2 center, float size);

        void Init();
        void Clear();

        /// @brief create new physic body and return its index in world's body container
        int Create_Body(const Body_Def& def);
        /// @brief get the refference physic body at index  
        Body* Get_Body(const int index);
        /// @brief remove physic body
        void Remove_Body(const int index);

        void GetQuadTreeDebug() { quad_tree.GetInfo(); };

        void Update();
        void Draw();

        bool TouchSolid_ibound(glm::ivec4 ibound);
        // return true if there is a collider that have all tag in tag, and no tag in null_tag
        bool BoxCast(glm::ivec4 ibound, uint64_t tag, uint64_t null_tag = 0);

        private:
        std::vector <int> solids, actors;
        bool body_added_or_removed = false;

        std::vector<int> Query(const irect& rect) { return quad_tree.Query(rect); };
        void GetSABodyList();
        void AddToUpdateList(int id);

        void ResolveSolid(int id);
        void ResolveActor(int id);

    };
}