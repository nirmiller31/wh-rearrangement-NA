#include "mconstraints.hpp"



MConstraints::MConstraints() : Constraints(false), obs_negative_vertex_constraints(),
                               obs_positive_vertex_constraints(), obs_last_time_negative_vertex_constraint(),
                               obs_last_time_positive_vertex_constraint(-1) {}



void MConstraints::add_negative_obs_vertex_constraint(VertexConstraint &v_constraint) {
    assert(obs_positive_vertex_constraints.find(v_constraint.time) == obs_positive_vertex_constraints.end() ||
           obs_positive_vertex_constraints.at(v_constraint.time) != v_constraint.location);
    auto it = obs_negative_vertex_constraints.find(v_constraint.time);
    if (it == obs_negative_vertex_constraints.end()) {
        obs_negative_vertex_constraints[v_constraint.time].insert(v_constraint.location);
    } else{
        it->second.insert(v_constraint.location);
    }
    auto it2 = obs_last_time_negative_vertex_constraint.find(v_constraint.location);
    if (it2 == obs_last_time_negative_vertex_constraint.end()){
        obs_last_time_negative_vertex_constraint.insert({v_constraint.location, v_constraint.time});
    } else if (it2->second < v_constraint.time) {
        it2->second = v_constraint.time;
    }
}


void MConstraints::add_positive_obs_vertex_constraint(VertexConstraint &v_constraint) {
    assert(obs_negative_vertex_constraints.find(v_constraint.time) == obs_negative_vertex_constraints.end() ||
           obs_negative_vertex_constraints.at(v_constraint.time).count(v_constraint.location) == 0);
    obs_positive_vertex_constraints[v_constraint.time] = v_constraint.location;
    if (obs_last_time_positive_vertex_constraint < v_constraint.time) {
        obs_last_time_positive_vertex_constraint = v_constraint.time;
    }
}


bool MConstraints::is_obs_state_valid(int time, const Location &obs_location) const {
    auto it = obs_negative_vertex_constraints.find(time);
    if (it != obs_negative_vertex_constraints.end() && it->second.find(obs_location) != it->second.end()) {
        return false;
    }

    auto it2 = obs_positive_vertex_constraints.find(time);
    if (it2 != obs_positive_vertex_constraints.end() && it2->second != obs_location) {
        return false;
    }

    return true;
}


bool MConstraints::can_stay(const MState &state) const {
    // assuming that obs location1 is the same as the state location1
    if (obs_last_time_positive_vertex_constraint > state.time) {
        return false;
    } else if (obs_last_time_positive_vertex_constraint == state.time) {
        auto it = obs_positive_vertex_constraints.find(state.time);
        assert(it != obs_positive_vertex_constraints.end());
        if (it->second != state.location) {
            return false;
        }
    }

    return Constraints::can_stay(state);
}
