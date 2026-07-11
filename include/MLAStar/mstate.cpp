#include "mstate.hpp"

MState::MState(int time, Location &location, int label) : State(time, location), label(label) {}

MState::MState(State &state, int label) : State(state), label(label) {}

bool MState::operator==(const MState &other) const {
    return State::operator==(other) && label == other.label;
}
