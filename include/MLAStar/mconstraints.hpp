#ifndef MAWR_MCONSTRAINTS_HPP
#define MAWR_MCONSTRAINTS_HPP

#include "../constraints/constraints.hpp"
#include "mstate.hpp"


class MConstraints : public Constraints {
private:
    using Constraints::can_stay;
public:
    unordered_map<int, unordered_set<Location>> obs_negative_vertex_constraints;
    unordered_map<int, Location> obs_positive_vertex_constraints;
    unordered_map<Location, int> obs_last_time_negative_vertex_constraint;
    int obs_last_time_positive_vertex_constraint;

public:
    MConstraints();
//    explicit MConstraints(const MConstraints &constraints, const VertexConstraint &v_constraint, bool is_positive);
    MConstraints(MConstraints& constraints) = default;

    void add_negative_obs_vertex_constraint(VertexConstraint &v_constraint);
    void add_positive_obs_vertex_constraint(VertexConstraint &v_constraint);

    bool is_obs_state_valid(int time, const Location& obs_location) const;
    bool can_stay(const MState &state) const;
};


#endif //MAWR_MCONSTRAINTS_HPP
