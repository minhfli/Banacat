#include "AABB_QuadTree.h"

#include <Graphics/2D_Renderer.h>

#include "Common/Common.h"
#include <queue>
#include <iostream>

namespace sk_physic2d {
    namespace { // some special funtion, only for this file
        irect GetQuadrant(const irect& r, const int quadrant) {
            glm::ivec2 center = r.centeri();

            //  +-------+-------+
            //  |   2   |   3   |
            //  +-------+-------+
            //  |   0   |   1   |
            //  +-------+-------+

            int x, y, z, w;

            if ((quadrant & 1) == 0) {
                x = r.bound.x;
                z = center.x;
            }
            else {
                x = center.x;
                z = r.bound.z;
            }
            if ((quadrant & 2) == 0) {
                y = r.bound.y;
                w = center.y;
            }
            else {
                y = center.y;
                w = r.bound.w;
            }

            return irect(x, y, z, w);
        }
        /// @brief return quadrant(number) that target rect is in, asume that target rect is in current rect 
        int FindQuadRant(const irect& r, const irect& target) {
            int x = -1;
            int y = -1;
            glm::ivec2 center = r.centeri();

            //  +-------+-------+
            //  |   2   |   3   |
            //  +-------+-------+
            //  |   0   |   1   |
            //  +-------+-------+

            if (target.bound.x >= center.x) x = 1;
            if (target.bound.z <= center.x) x = 0;

            if (target.bound.y >= center.y) y = 2;
            if (target.bound.w <= center.y) y = 0;

            if (x == -1 || y == -1) return -1;
            return x + y;
        }
    }

    void QuadTree::SplitNode(Node* node) {
        if (node->depth >= MAX_TREE_DEPTH) return;

        for (int i = 0;i <= 3; i++)
            node->child[i] = new Node(node, node->depth + 1, GetQuadrant(node->node_rect, i));

        auto new_value = std::vector<std::pair<int, irect>>();
        for (auto value : node->m_value) {
            int i = FindQuadRant(node->node_rect, value.second);
            if (i != -1) {
                node->child[i]->m_value.push_back(value);
                // update item-node map
                Item_Node_map[value.first] = node->child[i];
            }
            else new_value.push_back(value);
        }
        node->m_value = std::move(new_value);
    }
    void QuadTree::MergeNode(Node* node) {
        if (node == nullptr) return;
        if (
            !node->isleaf() &&
            node->child[0]->isleaf() &&
            node->child[1]->isleaf() &&
            node->child[2]->isleaf() &&
            node->child[3]->isleaf() &&

            node->m_value.size() +
            node->child[0]->m_value.size() +
            node->child[1]->m_value.size() +
            node->child[2]->m_value.size() +
            node->child[3]->m_value.size()
            <= NODE_MERGE_CAPACITY
        ) {
            for (int i = 0; i <= 3; i++) {
                for (auto& value : node->child[i]->m_value) {
                    // update item-node map
                    Item_Node_map[value.first] = node;
                    node->m_value.push_back(value);
                }
                delete node->child[i];
                node->child[i] = nullptr;
            }
           // std::cout << "Node Merged\n";
        }
        if (node->isleaf())
            MergeNode(node->parent);
    }

    void QuadTree::AddToNode(Node* node, const irect& rect, const int value) {

        //add value to root node even if rect is not inside root node bound
        //if (!node->node_rect.contain(rect)) return;

        if (node->isleaf()) {
            // add value to node
            if (node->depth >= MAX_TREE_DEPTH || node->m_value.size() < NODE_CAPACITY) {
                node->m_value.push_back({ value,rect });
                Item_Node_map[value] = node;
            }
            else {
                SplitNode(node);
                AddToNode(node, rect, value);
            }
        }
        else {
            // find children node to add value to
            int i = FindQuadRant(node->node_rect, rect);
            if (i != -1)
                AddToNode(node->child[i], rect, value);
            else {
                node->m_value.push_back({ value,rect });
                Item_Node_map[value] = node;

                // merge node if node is splited but cannot add pointer to node
                MergeNode(node);
            }
        }
    }
    void QuadTree::AddValue(const irect& rect, const int value) {
        return AddToNode(&this->root, rect, value);
    }

    void QuadTree::RemoveFromNode(Node* node, const int value) {
        // find value in node
        auto it = std::find_if(node->m_value.begin(), node->m_value.end(), [&value](const std::pair<int, irect>& _value) {
            return _value.first == value;
        });
        // if value is not found, return
        if (it == node->m_value.end()) {
            Error("QuadTree Cant find and remove item: " + std::to_string(value));
            return;
        }
        // swap value with the last
        *it = std::move(node->m_value.back());
        node->m_value.pop_back();

        // try merge node
        MergeNode(node);
    }
    void QuadTree::RemoveValue(const int value) {
        // find node that value is in
        auto key_pair = Item_Node_map.find(value);
        if (key_pair == Item_Node_map.end()) {
            Error("QuadTree Cant find and remove item: " + std::to_string(value));
            return;
        }
        Node* node = key_pair->second;
        Item_Node_map.erase(key_pair);

        RemoveFromNode(node, value);
    }

    void QuadTree::UpdateValue(const irect& rect, const int value) {
        // similar to RemoveValue() but dont delete from item-node map
        auto key_pair = Item_Node_map.find(value);
        if (key_pair == Item_Node_map.end()) {
            Error("QuadTree Cant find and remove item: " + std::to_string(value));
            return;
        }
        Node* node = key_pair->second;

        if (node->node_rect.contain(rect)) { //if node can still store value
            // find value in node
            auto it = std::find_if(node->m_value.begin(), node->m_value.end(), [&value](const std::pair<int, irect>& _value) {
                return _value.first == value;
            });
            it->second = rect;
        }
        else {
            RemoveFromNode(node, value);
            AddValue(rect, value);
        }
    }

    std::vector<int> QuadTree::Query(const irect& rect) {
        auto values = std::vector<int>();
        Query(values, &root, rect);
        return values;
    }
    void QuadTree::Query(std::vector<int>& m_vector, Node* node, const irect& rect) {
        if (node != &this->root && !node->node_rect.overlap(rect)) return;

        for (auto value : node->m_value) {
            if (rect.overlap(value.second))
                m_vector.push_back(value.first);
        }
        if (!node->isleaf()) {
            for (int i = 0; i <= 3; i++)
                Query(m_vector, node->child[i], rect);
        }
    }

    void QuadTree::Init(const irect& TREE_RECT) {
        root = Node(nullptr, 0, TREE_RECT);
    }

    void QuadTree::Clear() {
        ClearNode(&root);
        root.m_value.clear();
    }
    void QuadTree::ClearNode(Node* node) {
        if (node == nullptr) return;
        if (!node->isleaf()) {
            ClearNode(node->child[0]);
            ClearNode(node->child[1]);
            ClearNode(node->child[2]);
            ClearNode(node->child[3]);
            delete node->child[0]; node->child[0] = nullptr;
            delete node->child[1]; node->child[1] = nullptr;
            delete node->child[2]; node->child[2] = nullptr;
            delete node->child[3]; node->child[3] = nullptr;
        }
    }

    void QuadTree::Draw() {
        DrawNode(&root);
    }
    void QuadTree::DrawNode(Node* node) {
        sk_graphic::Renderer2D_AddBBox(node->node_rect.true_bound(), 1, glm::vec4(0, 0, 1, 1));
        if (!node->isleaf())
            for (int i = 0; i <= 3; i++) DrawNode(node->child[i]);
    }

    void QuadTree::GetInfo() {
        GetInfo(&root);
    }
    void QuadTree::GetInfo(Node* node) {

        std::cout << "Node rect: "
            << node->node_rect.bound.x << " "
            << node->node_rect.bound.y << " "
            << node->node_rect.bound.z << " "
            << node->node_rect.bound.w << "\n";
        std::cout << "Depth: " << node->depth << "  Value: \n";
        for (auto value : node->m_value) {
            std::cout << value.first << "   ";
            std::cout << value.second.bound.x << " ";
            std::cout << value.second.bound.y << " ";
            std::cout << value.second.bound.z << " ";
            std::cout << value.second.bound.w << "\n";
        }
        std::cout << '\n';
        if (!node->isleaf())
            for (auto childnode : node->child) {
                GetInfo(childnode);
            }

    }

}