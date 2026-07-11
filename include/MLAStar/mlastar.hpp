#ifndef MAWR_MLASTAR_HPP
#define MAWR_MLASTAR_HPP

#include "../common.hpp"
#include "mlastar_node.hpp"


class MLAStar {
private:
    vector<vector<CellType>> map;
    int map_rows;
    int map_cols;
    Location start_location;
    int start_time;
    Location pickup_location;
    Location delivery_location;
    const int dx[5] = {0, 0, 1, 0, -1};
    const int dy[5] = {0, 1, 0, -1, 0};

    typedef boost::heap::d_ary_heap<shared_ptr<MLAStarNode>, boost::heap::arity<2>, boost::heap::mutable_<true>,
            boost::heap::compare<MLAStarNodeCompare>> OpenList;

    typedef OpenList::handle_type NodeHandle;

public:
    MLAStar(int map_rows, int map_cols, vector<vector<CellType>> &map, State &start_state, Location &pickup_location,
            Location &delivery_location);

    ~MLAStar();

    std::pair<shared_ptr<TimedPath>, int> find_path(shared_ptr<MConstraints> &constraints);

private:
    void insert_initial_nodes(OpenList &open_list, unordered_map<MState, NodeHandle> &state_to_handle);

    int calculate_h(const Location &curr_location, int label) const;
};


#endif //MAWR_MLASTAR_HPP
