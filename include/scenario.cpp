#include "scenario.hpp"


Scenario::Scenario(int map_rows, int map_cols, vector<vector<CellType>>& map,
                   vector<Location>& agent_start_locations, int num_of_tasks,
                   vector<Location>& pickup_locations, vector<Location>& delivery_locations) :
        map_rows(map_rows), map_cols(map_cols), map(map),
        num_of_agents(static_cast<int>(agent_start_locations.size())),
        num_of_obstacles(static_cast<int>(pickup_locations.size())),
        num_of_tasks(num_of_tasks),
        agent_start_locations(agent_start_locations),
        pickup_locations(pickup_locations), delivery_locations(delivery_locations) {
    for (int i = 0; i < map_rows; i++) {
        for (int j = 0; j < map_cols; j++) {
            if (map[i][j] == STATIC_OBSTACLE) {
                static_obstacles.insert(Location(i, j));
            }
        }
    }
}

Scenario::~Scenario() {
    map.clear();
    agent_start_locations.clear();
    pickup_locations.clear();
    delivery_locations.clear();
    static_obstacles.clear();
}

std::ostream &operator<<(std::ostream &os, const Scenario &s) {
    os << "Map size: " << s.map_rows << "x" << s.map_cols << std::endl
       << "Num of agents: " << s.num_of_agents << std::endl
       << "Num of obstacles: " << s.num_of_obstacles << std::endl
       << "Num of tasks: " << s.num_of_tasks << std::endl
       << "Agent start locations: " << "[" ;
    for (int i = 0; i < s.num_of_agents; i++) {
        os << s.agent_start_locations[i];
        if (i < s.num_of_agents - 1) {
            os << ", ";
        }
    }
    os << "]" << std::endl << "Pickup locations: " << "[";
    for (int i = 0; i < s.num_of_tasks; i++) {
        os << s.pickup_locations[i];
        if (i < s.num_of_tasks - 1) {
            os << ", ";
        }
    }
    os << "]" << std::endl << "Delivery locations: " << "[";
    for (int i = 0; i < s.num_of_tasks; i++) {
        os << s.delivery_locations[i];
        if (i < s.num_of_tasks - 1) {
            os << ", ";
        }
    }
    os << "]" << std::endl;
    return os;
}


