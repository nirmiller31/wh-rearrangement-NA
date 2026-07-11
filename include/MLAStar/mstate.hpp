#ifndef MAWR_MSTATE_HPP
#define MAWR_MSTATE_HPP

#include "../common.hpp"


class MState : public State {
public:
    const int label;

    MState(int time, Location& location, int label);
    MState(State& state, int label);

    bool operator==(const MState& other) const;

    inline friend size_t hash_value(MState const& state){
        size_t seed = 0;
        boost::hash_combine(seed, state.time);
        boost::hash_combine(seed, state.location);
        boost::hash_combine(seed, state.label);
        return seed;
    }

};



#endif //MAWR_MSTATE_HPP
