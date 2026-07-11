#include "natcbs_node.hpp"


NATCBSNode::NATCBSNode(int idx, vector<shared_ptr<TimedPath>> &obs_paths, int cost,
                       vector<shared_ptr<Constraints>> &constraint_table,
                       const shared_ptr<NATCBSNode>& parent): idx(idx),
        obs_paths(obs_paths), cost(cost),
        constraint_table(constraint_table),
        iteration(-1), parent(parent) {}

NATCBSNode::NATCBSNode(int idx, const shared_ptr<NATCBSNode> &node) :
        NATCBSNode(idx, node->obs_paths, node->cost,
                   node->constraint_table) {}
//                   node->constraint_table, node) {}

shared_ptr<Conflict> NATCBSNode::get_first_obs_conflict() const {
    int last_t = 0;
    int first_t = std::numeric_limits<int>::max();
    for (const auto &obs_path : obs_paths) {
        last_t = std::max(last_t, obs_path->back().time);
        first_t = std::min(first_t, obs_path->front().time);
    }
    auto num_of_obs = obs_paths.size();

    for (int t = first_t; t <= last_t; t++) {
        for (size_t i = 0; i < num_of_obs; i++) {
            const Location &loc_i = obs_paths[i]->get_location_at_time(t);
            for (size_t j = i + 1; j < num_of_obs; j++) {
                if ((t < obs_paths[i]->front().time && t < obs_paths[j]->front().time) ||
                    (t >= obs_paths[i]->back().time && t >= obs_paths[j]->back().time)) {
                    continue;
                }

                //  check vertex conflict
                const Location &loc_j = obs_paths[j]->get_location_at_time(t);

                if (loc_i == loc_j) {
                    return std::make_shared<Conflict>(t, i, j, loc_i);
                }

                if (t == last_t || t < obs_paths[i]->front().time || t < obs_paths[j]->front().time ||
                    t >= obs_paths[i]->back().time || t >= obs_paths[j]->back().time) {
                    continue;
                }

                // check edge conflict
                const Location &loc_i_next = obs_paths[i]->get_location_at_time(t + 1);
                const Location &loc_j_next = obs_paths[j]->get_location_at_time(t + 1);
                if (loc_i == loc_j_next && loc_j == loc_i_next) {
                    return std::make_shared<Conflict>(t, i, j, loc_i, loc_i_next);
                }
            }
        }
    }

    return nullptr;
}
