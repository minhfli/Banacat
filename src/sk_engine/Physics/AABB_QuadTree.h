#pragma once

#define GLM_FORCE_SWIZZLE //enable swizzle for glm
#include <GLM/glm.hpp>

namespace sk_physic2d {
    const int32_t MAX_NODE_COUNT = 1000000; //1e6
    const int32_t MAX_TREE_DEPTH = 7;
    const int32_t NODE_CAPACITY = 10;

    const glm::vec2 MIN_NODE_SIZE = glm::vec2(1);


    class QuadTree {

        struct Node {
            // node depth
            int depth = 0;

            // parent node index
            int parent = -1;
            // lower left child node index
            int child = -1;

            inline int lower_left() const { return child + 0; };
            inline int lower_right() const { return child + 1; };
            inline int upper_left() const { return child + 2; };
            inline int upper_right() const { return child + 3; };

            glm::vec2 center;   // center pos
            glm::vec2 size;
            glm::vec4 bound;    // rectangle bound

            /// fast function to set node rectangle data
            inline void set_center_size(const glm::vec2 c, const glm::vec2 s) {
                center = c;
                size = s;
                bound = glm::vec4(c - s / 2.0f, c + s / 2.0f);
            }
            inline void set_bound(const glm::vec4 b) {
                bound = b;
                center = (glm::vec2(b.x, b.y) + glm::vec2(b.z, b.w)) / 2.0f;
                size = glm::vec2(b.z, b.w) - glm::vec2(b.x, b.y);
            }
            inline void set_bound(glm::vec2 p1, glm::vec2 p2) {
                if (p1.x > p2.x) std::swap(p1.x, p2.x);
                if (p1.y > p2.y) std::swap(p1.y, p2.y);
                bound = glm::vec4(p1, p2);
                center = (p1 + p2) / 2.0f;
                size = p2 - p1;
            }

            // list of rectangle in the tree 
            int rect_id[NODE_CAPACITY];
            int local_count = 0;
            int global_count = 0;
        };
        private:

        Node NodeList[MAX_NODE_COUNT];
        int node_counts = 1;

        public:
        glm::vec4 tree_bound = glm::vec4(-64, -64, 64, 64);
        glm::vec2 min_node_size = glm::vec2(2, 2);

        void Build(glm::vec2 tree_center = glm::vec2(0), glm::vec2 tree_size = glm::vec2(64), int max_depth = 10);

        int Find_Node(glm::vec4 rect);
        void Add_to_Node(glm::vec4 rect, int node_index, int collider_id);

        void AddRect(glm::vec4 rect, int collider_id);
        void RmvRect(glm::vec4 rect, int collider_id);

        void Update();
        void Draw();
    };

}