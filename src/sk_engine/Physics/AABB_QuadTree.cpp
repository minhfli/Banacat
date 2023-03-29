#include "AABB_QuadTree.h"

#include <Graphics/2D_Renderer.h>

#include "Common/Common.h"
#include <queue>
#include <iostream>

namespace sk_physic2d {
    namespace { // some special funtion, only for this file
        rect GetQuadrant(const rect& r, const int quadrant) {
            glm::vec2 center;
            glm::vec2 hsize = r.hsize / 2.0f;

            //  +-------+-------+
            //  |   2   |   3   |
            //  +-------+-------+
            //  |   0   |   1   |
            //  +-------+-------+

            // quad rant in left or right
            if ((quadrant & 1) == 0)
                center.x = r.pos.x - hsize.x;
            else center.x = r.pos.x + hsize.x;
            // quad rant is below or up
            if ((quadrant & 2) == 0)
                center.y = r.pos.y - hsize.y;
            else center.y = r.pos.y + hsize.y;

            return rect(center, r.hsize);
        }
        /// @brief return quadrant(number) that target rect is in, asume that target rect is in current rect 
        int FindQuadRant(const rect& r, const rect& target) {
            int quadrant = 0;
            if (target.pos.x > r.pos.x) quadrant |= 1;
            if (target.pos.y > r.pos.y) quadrant |= 2;

            rect quadrant_rect = GetQuadrant(r, quadrant);

            if (quadrant_rect.contain(target)) return quadrant;
            return -1;
        }
    }

    void QuadTree::SplitNode(Node* node) {
        if (node->depth >= MAX_TREE_DEPTH) return;

        for (int i = 0;i <= 3; i++)
            node->child[i] = new Node(node, node->depth + 1, GetQuadrant(node->node_rect, i));

        auto new_value = std::vector<std::pair<int, rect>>();
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
            <= NODE_CAPACITY
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
            std::cout << "Node Merged\n";
        }
        if (node->isleaf())
            MergeNode(node->parent);
    }

    void QuadTree::AddToNode(Node* node, const rect& rect, const int value) {

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
            }
        }
    }
    void QuadTree::AddValue(const rect& rect, const int value) {
        return AddToNode(&this->root, rect, value);
    }

    void QuadTree::RemoveFromNode(Node* node, const int value) {
        // find value in node
        auto it = std::find_if(node->m_value.begin(), node->m_value.end(), [&value](const std::pair<int, rect>& _value) {
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

    void QuadTree::UpdateValue(const rect& rect, const int value) {
        // similar to RemoveValue() but dont delete from item-node map
        auto key_pair = Item_Node_map.find(value);
        if (key_pair == Item_Node_map.end()) {
            Error("QuadTree Cant find and remove item: " + std::to_string(value));
            return;
        }
        Node* node = key_pair->second;

        RemoveFromNode(node, value);
        AddValue(rect, value);
    }

    std::vector<int> QuadTree::Query(const rect& rect) {
        auto values = std::vector<int>();
        Query(values, &root, rect);
        return values;
    }
    void QuadTree::Query(std::vector<int>& m_vector, Node* node, const rect& rect) {
        if (!node->node_rect.overlap(rect)) return;

        for (auto value : node->m_value) {
            if (rect.overlap(value.second))
                m_vector.push_back(value.first);
        }
        if (!node->isleaf()) {
            for (int i = 0; i <= 3; i++)
                Query(m_vector, node->child[i], rect);
        }
    }

    void QuadTree::Init(const rect& TREE_RECT) {
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
            delete node->child[0];
            delete node->child[1];
            delete node->child[2];
            delete node->child[3];
        }
    }

    void QuadTree::Draw() {
        DrawNode(&root);
    }
    void QuadTree::DrawNode(Node* node) {
        sk_graphic::Renderer2D_AddBBox(node->node_rect.bound(), 1, glm::vec4(0, 0, 1, 1));
        if (!node->isleaf())
            for (int i = 0; i <= 3; i++) DrawNode(node->child[i]);
    }

    void QuadTree::GetInfo() { GetInfo(&root); }
    void QuadTree::GetInfo(Node* node) {

        std::cout << "Depth: " << node->depth << "  Value: ";
        for (auto value : node->m_value) {
            std::cout << value.first << " ";
        }
        std::cout << '\n';
        if (!node->isleaf())
            for (auto childnode : node->child) {
                GetInfo(childnode);
            }

    }

}