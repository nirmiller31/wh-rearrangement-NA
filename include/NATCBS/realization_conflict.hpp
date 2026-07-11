#ifndef MAWR_REALIZATION_CONFLICT_HPP
#define MAWR_REALIZATION_CONFLICT_HPP

#include "../common.hpp"

struct RealizationConflict{
    int obs_id;
    Location location1;
    Location location2;
    int time;

    RealizationConflict(int obs_id, Location location1, Location location2, int time) : obs_id(obs_id), location1(location1), location2(location2), time(time) {}

    friend std::ostream &operator<<(std::ostream &os, const RealizationConflict &c) {
        return os << "RealizationConflict: obs: " << c.obs_id << " edge: " << c.location1 << "->" << c.location2 << " time: " << c.time;
    }
};

#endif //MAWR_REALIZATION_CONFLICT_HPP
