#ifndef MAWR_NATCBS_HPP
#define MAWR_NATCBS_HPP

#include "../scenario.hpp"
#include "natcbs_node.hpp"
#include "../AStar/astar.hpp"
#include "f_agents_planner.hpp"
#include "realization_conflict.hpp"

typedef FAgentsPlanner AgentsPlanner;

class NATCBS {
private:
    int num_of_agents;
    int num_of_tasks;
    bool verbose;
    int iterations = 0;
    int node_count = 0;

    vector<int> obs_start_ts;
    vector<AStar> solvers;

    AgentsPlanner agents_planner;

    typedef boost::heap::d_ary_heap<shared_ptr<NATCBSNode>, boost::heap::arity<2>, boost::heap::mutable_<true>,
            boost::heap::compare<NATCBSNodeCompare>> OpenList;

public:
    NATCBS(int map_rows, int map_cols, vector<vector<CellType>> &map, vector<Location> &agent_start_locations,
               vector<Location> &pickup_locations, vector<Location> &delivery_locations,
               bool verbose);
    NATCBS(Scenario &scenario, bool verbose);

    shared_ptr<Plan> solve(int time_limit, bool& time_limit_exceeded);
    shared_ptr<Plan> solve(int time_limit = 60);


private:
    shared_ptr<NATCBSNode> get_root_node();
    shared_ptr<TimedPath> plan_single_obs_path(int obs_id, shared_ptr<Constraints> constraints);

    std::pair<shared_ptr<NATCBSNode>, shared_ptr<NATCBSNode>>
    resolve_obs_conflict(const shared_ptr<NATCBSNode>& node, const shared_ptr<Conflict>& conflict);
    std::pair<shared_ptr<NATCBSNode>, shared_ptr<NATCBSNode>>
    resolve_realization_conflict(const shared_ptr<NATCBSNode>& node, const shared_ptr<RealizationConflict>& conflict);
};



#endif //MAWR_NATCBS_HPP
