#ifndef MAWR_MLASTAR_NODE_HPP
#define MAWR_MLASTAR_NODE_HPP

#include "../common.hpp"
#include "../AStar/astar_node.hpp"
#include "mconstraints.hpp"



class MLAStarNode {
public:
    MState state;
    int g_value;
    int f_value;
    int pick_up_time;
    shared_ptr<MLAStarNode> parent;

    MLAStarNode(MState& state, int g_value, int f_value, int pick_up_time = -1);
    MLAStarNode(MState& state, int g_value, int f_value, int pick_up_time, shared_ptr<MLAStarNode>& parent);

    shared_ptr<TimedPath> get_path() const;
};


struct MLAStarNodeCompare{
    bool operator()(const shared_ptr<MLAStarNode>& lhs, const shared_ptr<MLAStarNode>& rhs) const {
        return (lhs->f_value > rhs->f_value) || (lhs->f_value == rhs->f_value && lhs->g_value < rhs->g_value);
    }
};


#endif //MAWR_MLASTAR_NODE_HPP
