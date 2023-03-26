#include "AABB_QuadTree.h"

#include <queue>

namespace sk_physic2d {
    void QuadTree::Build(glm::vec2 tree_center, glm::vec2 tree_size, int max_depth) {

        // root node
        NodeList[0].center = tree_center;
        NodeList[0].size = tree_size;
        NodeList[0].bound = glm::vec4(tree_center - tree_size / 2.0f, tree_center + tree_size / 2.0f);
        node_counts = 1;

        std::queue<int> node_to_process;
        node_to_process.push(0);
        while (!node_to_process.empty()) {
            int cur_index = node_to_process.front();
            node_to_process.pop();

            Node& cur_node = NodeList[cur_index];

            if (cur_node.depth >= max_depth) continue;

            cur_node.child = node_counts;
            node_counts += 4;

            Node& LL_node = NodeList[cur_node.lower_left()];
            Node& LR_node = NodeList[cur_node.lower_right()];
            Node& UL_node = NodeList[cur_node.upper_left()];
            Node& UR_node = NodeList[cur_node.upper_right()];

            LL_node.parent = cur_index;
            LR_node.parent = cur_index;
            UL_node.parent = cur_index;
            UR_node.parent = cur_index;

            LL_node.set_bound(cur_node.center, cur_node.bound.xy());
            LR_node.set_bound(cur_node.center, cur_node.bound.zy());
            UL_node.set_bound(cur_node.center, cur_node.bound.xw());
            UR_node.set_bound(cur_node.center, cur_node.bound.zw());
        }
    }
}