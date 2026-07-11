#include "mlastar.hpp"




MLAStar::MLAStar(int map_rows, int map_cols, vector<vector<CellType>> &map, State &start_state,
                 Location &pickup_location, Location &delivery_location) : map(map), map_rows(map_rows),
                 map_cols(map_cols), start_location(start_state.location), start_time(start_state.time),
                 pickup_location(pickup_location), delivery_location(delivery_location) {}


MLAStar::~MLAStar() = default;


std::pair<shared_ptr<TimedPath>, int> MLAStar::find_path(shared_ptr<MConstraints> &constraints) {
    OpenList open_list;
    unordered_map<MState, NodeHandle> state_to_handle;
    unordered_set<MState> closed_list;
    insert_initial_nodes(open_list, state_to_handle);

    while (!open_list.empty()){
        shared_ptr<MLAStarNode> curr_node = open_list.top();
        if (curr_node->state.label == 1 && curr_node->state.location == delivery_location &&
            constraints->can_stay(curr_node->state)){
            return std::make_pair(curr_node->get_path(), curr_node->pick_up_time);
        }

        open_list.pop();
        state_to_handle.erase(curr_node->state);
        closed_list.insert(curr_node->state);
        for (int i=0; i< 5; i++){
            Location next_location = Location(curr_node->state.location.x + dx[i], curr_node->state.location.y + dy[i]);
            if (next_location.x < 0 || next_location.x >= map_rows || next_location.y < 0 || next_location.y >= map_cols ||
                map[next_location.x][next_location.y] == CellType::BLOCKED ||
                (curr_node->state.label > 0 && map[next_location.x][next_location.y] == CellType::STATIC_OBSTACLE)){
                continue;
            }

            // same label
            MState next_state(curr_node->state.time + 1, next_location, curr_node->state.label);
            const Location &obs_location = curr_node->state.label == 0 ? pickup_location : next_location;
            int next_g = curr_node->g_value + 1;
            int next_f = -1;
            if ((!constraints->is_transition_valid(curr_node->state, next_state)) ||
                (!constraints->is_state_valid(next_state))) continue;

            if (closed_list.find(next_state) == closed_list.end() &&
                constraints->is_obs_state_valid(next_state.time, obs_location)){
                auto it = state_to_handle.find(next_state);
                if (it == state_to_handle.end()){
                    next_f = next_g + calculate_h(next_location, curr_node->state.label);
                    shared_ptr<MLAStarNode> next_node = std::make_shared<MLAStarNode>(next_state, next_g, next_f, curr_node->pick_up_time, curr_node);
                    NodeHandle handle = open_list.push(next_node);
                    state_to_handle.insert(std::make_pair(next_state, handle));
                } else {
                    NodeHandle handle = it->second;
                    if (next_g < (*handle)->g_value) {
                        int delta_g = (*handle)->g_value - next_g;
                        (*handle)->g_value = next_g;
                        (*handle)->f_value -= delta_g;
                        (*handle)->parent = curr_node;
                        open_list.increase(handle);
                    }
                }
            }

            if (i == 0 ||  // stay action
                curr_node->state.label > 0 || curr_node->state.location != pickup_location ||
                map[next_location.x][next_location.y] == CellType::STATIC_OBSTACLE) continue;

            // pickup obs
            MState next_state_pickup(curr_node->state.time + 1, next_location, 1);
            if (closed_list.find(next_state_pickup) == closed_list.end() &&
                constraints->is_obs_state_valid(next_state_pickup.time, next_location)){
                auto it = state_to_handle.find(next_state_pickup);
                if (it == state_to_handle.end()){
                    next_f = next_g + calculate_h(next_location, 1);
                    shared_ptr<MLAStarNode> next_node = std::make_shared<MLAStarNode>(next_state_pickup, next_g, next_f, curr_node->state.time, curr_node);
                    NodeHandle handle = open_list.push(next_node);
                    state_to_handle.insert(std::make_pair(next_state_pickup, handle));
                } else {
                    NodeHandle handle = it->second;
                    if (next_g < (*handle)->g_value) {
                        int delta_g = (*handle)->g_value - next_g;
                        (*handle)->g_value = next_g;
                        (*handle)->f_value -= delta_g;
                        (*handle)->parent = curr_node;
                        open_list.increase(handle);
                    }
                }
            }
        }
    }

    return {nullptr, -1};
}


void MLAStar::insert_initial_nodes(OpenList &open_list, unordered_map<MState, NodeHandle> &state_to_handle) {
    MState init_state(start_time, start_location, 0);
    int h = calculate_h(start_location, 0);
    shared_ptr<MLAStarNode> init_node = std::make_shared<MLAStarNode>(init_state, 0, h);
    state_to_handle.emplace(init_state, open_list.push(init_node));

    if (start_location != pickup_location) return;

    MState second_init_state(start_time, start_location, 1);
    shared_ptr<MLAStarNode> second_init_node = std::make_shared<MLAStarNode>(second_init_state, 0, h, 0);
    state_to_handle.emplace(second_init_state, open_list.push(second_init_node));
}


int MLAStar::calculate_h(const Location &curr_location, const int label) const {
    assert(label == 0 || label == 1);
    int distance = 0;
    if (label == 0) {
        distance += manhattan_distance(curr_location, pickup_location);
        distance += manhattan_distance(pickup_location, delivery_location);
    } else { // label == 1
        distance += manhattan_distance(curr_location, delivery_location);
    }
    return distance;
}


