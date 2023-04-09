#pragma once

#define GLM_FORCE_SWIZZLE //enable swizzle for glm
#include <GLM/glm.hpp>

#include <unordered_map>
#include <vector>

#include "Collider.h"

namespace sk_physic2d {
    const int32_t MAX_TREE_DEPTH = 10;
    const int32_t NODE_CAPACITY = 10;

    const glm::vec2 MIN_NODE_SIZE = glm::vec2(1);

    /// @brief quad tree is mainly used for indexing to a container
    class QuadTree {

        friend class AABB_World;
        struct Node {
            // node depth
            int depth = 0;
            // only use to know parent, dont use to change parent's value
            Node* parent = nullptr;
            Node* child[4] = { nullptr,nullptr,nullptr,nullptr };
            irect node_rect;

            // list of rectangle in the tree 
            // not a array but a pointer to reduce node size 
            std::vector <std::pair<int, irect>> m_value;

            inline bool isleaf() const {
                return child[0] == nullptr;
            }
            Node() {}
            Node(Node* p, int d, irect r): parent(p), depth(d), node_rect(r) {
                m_value.reserve(NODE_CAPACITY);
            }
        };

        //? data -----------------------------------------------------------------------------------
        private:
        std::unordered_map <int, Node*> Item_Node_map;
        Node root;

        //? funtions -------------------------------------------------------------------------------
        private:
        void MergeNode(Node* node);
        void SplitNode(Node* node);
        void AddToNode(Node* node, const irect& rect, const int value);
        void RemoveFromNode(Node* node, const int value);
        void RemoveFromNode(Node* node, const irect& rect, const int value);
        void Query(std::vector<int>& values, Node* node, const irect& rect);

        void GetInfo(Node* node);
        void DrawNode(Node* node);

        void ClearNode(Node* node);

        public:
        QuadTree() {}
        void Init(const irect& tree_RECT);

        void AddValue(const irect& rect, const int value);
        void RemoveValue(const int value);
        void UpdateValue(const irect& rect, const int value);

        std::vector<int> Query(const irect& rect);

        void Clear();

        void GetInfo();
        void Draw();
    };

}