#ifndef MAWR_NATCBS_NODE_HPP
#define MAWR_NATCBS_NODE_HPP

#include "../common.hpp"
#include "../constraints/constraints.hpp"

class NATCBSNode {
public:
    int idx;
    vector<shared_ptr<TimedPath>> obs_paths;
    int cost;
    vector<shared_ptr<Constraints>> constraint_table;

    int iteration;
    const shared_ptr<NATCBSNode> parent;

    NATCBSNode(int idx, vector<shared_ptr<TimedPath>>& obs_paths, int cost,
               vector<shared_ptr<Constraints>> &constraint_table,
               const shared_ptr<NATCBSNode>& parent = nullptr);
    explicit NATCBSNode(int idx, const shared_ptr<NATCBSNode>& node);

    shared_ptr<Conflict> get_first_obs_conflict() const;
};

struct NATCBSNodeCompare{
    bool operator()(const shared_ptr<NATCBSNode>& lhs, const shared_ptr<NATCBSNode>& rhs) const {
//        return lhs->cost > rhs->cost;
        return std::tie(lhs->cost, lhs->idx) > std::tie(rhs->cost, rhs->idx);
    }
};

#endif //MAWR_NATCBS_NODE_HPP
