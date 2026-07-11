#include "mlastar_node.hpp"



MLAStarNode::MLAStarNode(MState& state, int g_value, int f_value, int pick_up_time) :
        state(state), g_value(g_value), f_value(f_value), pick_up_time(pick_up_time),
        parent(nullptr) {}

MLAStarNode::MLAStarNode(MState &state, int g_value, int f_value, int pick_up_time, shared_ptr<MLAStarNode> &parent) :
        state(state), g_value(g_value), f_value(f_value), pick_up_time(pick_up_time),
        parent(parent) {}

shared_ptr<TimedPath> MLAStarNode::get_path() const {
    shared_ptr<TimedPath> path = std::make_shared<TimedPath>();
    path->push_back(this->state);
    shared_ptr<MLAStarNode> current = this->parent;
    while (current) {
        path->push_back(current->state);
        current = current->parent;
    }
    std::reverse(path->begin(), path->end());
    return path;
}
