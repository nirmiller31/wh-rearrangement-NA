#ifndef MAWR_SCENARIO_HPP
#define MAWR_SCENARIO_HPP

#include "common.hpp"


class Scenario {
public:
    int map_rows;
    int map_cols;
    vector<vector<CellType>> map;

    int num_of_agents;
    int num_of_obstacles;
    int num_of_tasks;

    vector<Location> agent_start_locations;
    vector<Location> pickup_locations;
    vector<Location> delivery_locations;
    unordered_set<Location> static_obstacles;

    Scenario(int map_rows, int map_cols, vector<vector<CellType>> &map, vector<Location> &agent_start_locations,
        int num_of_tasks, vector<Location> &pickup_locations, vector<Location> &delivery_locations);
    ~Scenario();
    friend std::ostream &operator<<(std::ostream &os, const Scenario &s);
};


#endif //MAWR_SCENARIO_HPP
