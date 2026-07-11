#include "common.hpp"


int Plan::get_makespan() {
    if (makespan != 0) {
        return makespan;
    }
    for (const auto &path : obs_paths) {
        if (path->size() > 1) {
            makespan = std::max(makespan, path->back().time);
        }
    }
    return makespan;
}

bool Plan::validate_plan(int map_rows, int map_cols, const vector<vector<CellType>> &map,
                         const vector<Location> &agent_start_locations, const vector<Location> &pickup_locations,
                         const vector<Location> &delivery_locations) {
    assert(pickup_locations.size() == delivery_locations.size());
    if (agent_paths.size() != agent_start_locations.size()) {
        return false;
    }
    if (obs_paths.size() != pickup_locations.size()) {
        return false;
    }
    unordered_map<int, unordered_set<std::pair<Location, Location>>> agent_edges;
    for (size_t i = 0; i < agent_paths.size(); ++i) {
        const auto &path = agent_paths.at(i);
        if (path->empty() || path->get_location_at_time(0) != agent_start_locations.at(i)) {
            return false;
        }
        for (int t=1; t<= this->get_makespan(); t++){
            Location loc = path->get_location_at_time(t);
            if (loc.x < 0 || loc.x >= map_rows || loc.y < 0 || loc.y >= map_cols || map[loc.x][loc.y] == CellType::BLOCKED) {
                return false;
            }
            Location prev_loc = path->get_location_at_time(t-1);
            if (prev_loc != loc){
                agent_edges[t - 1].insert({prev_loc, loc});
            }

            for (size_t j = i + 1; j < agent_paths.size(); ++j) {
                const auto &other_path = agent_paths.at(j);
                if ((other_path->get_location_at_time(t) == loc) || (other_path->get_location_at_time(t - 1) == loc && other_path->get_location_at_time(t) == prev_loc)) {
                    return false;
                }
            }
        }
    }

    for (size_t i = 0; i < obs_paths.size(); ++i) {
        const auto &path = obs_paths.at(i);
        if (path->empty() || path->get_location_at_time(0) != pickup_locations.at(i) || path->back().location != delivery_locations.at(i)) {
            return false;
        }
        for (int t=1; t<= path->back().time; t++){
            Location loc = path->get_location_at_time(t);
            if (loc.x < 0 || loc.x >= map_rows || loc.y < 0 || loc.y >= map_cols || map[loc.x][loc.y] == CellType::BLOCKED ||
                map[loc.x][loc.y] == CellType::STATIC_OBSTACLE) {
                return false;
            }
            Location prev_loc = path->get_location_at_time(t-1);
            if (prev_loc != loc && agent_edges[t - 1].find({prev_loc, loc}) == agent_edges[t - 1].end()){
                return false;
            }

            for (size_t j = i + 1; j < obs_paths.size(); ++j) {
                const auto &other_path = obs_paths.at(j);
                if ((other_path->back().time >= t && other_path->get_location_at_time(t) == loc) ||
                    (other_path->get_location_at_time(t - 1) == loc && other_path->get_location_at_time(t) == prev_loc)) {
                    return false;
                }
            }
        }
    }
    return true;
}
