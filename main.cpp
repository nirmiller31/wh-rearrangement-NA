#include <iostream>
#include <boost/program_options.hpp>
#include "include/common.hpp"
#include "include/scenario.hpp"
#include "include/NATCBS/natcbs.hpp"
#include <cstdlib>
#include <iomanip>
#include <filesystem>


auto parse_args(int argc, char** argv){
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    constexpr static const char* allowed_solvers[] = {"NATCBS"};
    desc.add_options()
        ("map,m", po::value<std::string>()->required(), "map file ([map_name].map)")
        ("scenario,s", po::value<std::string>()->required(), "scenario file (.scen)")
        ("alg,a", po::value<std::string>()->required(), "solver's algorithm to use (NATCBS)")
        ("time,t", po::value<int>()->default_value(60), "time limit in seconds (default 60)")
        ("output,o", po::value<std::string>()->default_value("results.csv"), "results output file (default results.csv)")
        ("v", po::value<int>()->default_value(1), "verbosity level: 0 - minimal, 1 - normal (default), 2 - debug")
        ("help", "produce help message");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.contains("help")) {
            std::cout << desc << std::endl;
            exit(0);
        }

        if (std::count(std::begin(allowed_solvers), std::end(allowed_solvers), vm["alg"].as<string>()) == 0){
            throw po::error("invalid solver");
        }

        if (vm["v"].as<int>() < 0 || vm["v"].as<int>() > 2){
            throw po::error("invalid verbosity level");
        }

    } catch (po::error &e) {
        std::cerr << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        exit(-1);
    }
    return vm;
}

shared_ptr<Scenario> read_scenario(const string& map_file, const string& scenario_file){
    std::ifstream map_stream(map_file);

    if (!map_stream) {
        std::cerr << "Unable to open map file" << std::endl;
        exit(1);
    }

    int map_rows, map_cols;
    map_stream >> map_rows >> map_cols;
    vector<vector<CellType>> map(map_rows, vector<CellType>(map_cols, PASSABLE));

    unordered_set<Location> static_obstacles;
    string row;
    for (int i = 0; i < map_rows; ++i) {
        try {
            map_stream >> row;
        } catch (std::exception &e) {
            std::cerr << "Map file is corrupted! (rows)" << std::endl;
            std::cerr << e.what() << std::endl;
            exit(1);
        }
        if (static_cast<int>(row.size()) != map_cols) {
            std::cerr << "Map file is corrupted! (cols)" << std::endl;
            exit(1);
        }
        for (int j = 0; j < map_cols; ++j) {
            switch (row[j]) {
                case '#':
                    map[i][j] = STATIC_OBSTACLE;
                    static_obstacles.insert(Location(i, j));
                    break;
                case '@':
                    map[i][j] = BLOCKED;
                    break;

                case '.': // passable
                default:
                    break;
            }
        }
    }

    std::ifstream scenario_stream(scenario_file);
    if (!scenario_stream) {
        std::cerr << "Unable to open scenario file" << std::endl;
        exit(1);
    }

    int num_of_agents, num_of_obstacles, num_of_tasks;
    scenario_stream >> num_of_agents >> num_of_obstacles >> num_of_tasks;

    if (num_of_tasks > num_of_obstacles) {
        std::cerr << "Scenario file is corrupted! (num of tasks)" << std::endl;
        exit(1);
    }

    vector<Location> agent_start_locations(num_of_agents);
    for (int i = 0; i < num_of_agents; ++i) {
        try {
            scenario_stream >> agent_start_locations[i].y >> agent_start_locations[i].x;
        } catch (std::exception &e) {
            std::cerr << "Scenario file is corrupted! (agent start locations)" << std::endl;
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }

    int task_count = 0;
    vector<Location> pickup_locations(num_of_obstacles), delivery_locations(num_of_obstacles);
    for (int i = 0; i < num_of_obstacles; ++i) {
        try {
            scenario_stream >> pickup_locations[i].y >> pickup_locations[i].x;
            scenario_stream >> delivery_locations[i].y >> delivery_locations[i].x;
            task_count += (pickup_locations[i] != delivery_locations[i]);
        } catch (std::exception &e) {
            std::cerr << "Scenario file is corrupted! (obstacles)" << std::endl;
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
    if (task_count != num_of_tasks) {
        std::cerr << "Scenario file is corrupted! (num of tasks)" << std::endl;
        exit(1);
    }

    return std::make_shared<Scenario>(map_rows, map_cols, map, agent_start_locations, num_of_tasks,
                                      pickup_locations, delivery_locations);
}


bool read_csv_content(const string& file_name, unordered_set<std::tuple<int, int, int, int>>& data){
    std::ifstream file(file_name);
    if (!file) {
        // new file
        return false;
    }

    string line;
    char c;
    int n_agents, n_obs, n_tasks, scen_n;
    bool header = true;
    while (std::getline(file, line)) {
        if (header) {
            header = false;
            continue;
        }
        std::istringstream iss(line);
        if (!(iss >> n_agents >> c >> n_obs >> c >> n_tasks >> c >> scen_n)) {
            std::cerr << "Error while reading file" << std::endl;
            exit(1);
        }
        data.insert(std::make_tuple(n_agents, n_obs, n_tasks, scen_n));
    }
    return true;
}


int main(int argc, char** argv) {
    const auto vm = parse_args(argc, argv);
    const string map_file = vm["map"].as<std::string>();
    const string scenario_file = vm["scenario"].as<std::string>();
    const string solver_name = vm["alg"].as<std::string>();
    const int time_limit = vm["time"].as<int>();
    const string results_csv_file = vm["output"].as<std::string>();
    const int v_level = vm["v"].as<int>();

    std::ofstream out_csv;
    out_csv.open(results_csv_file, std::ios::app);
    if (std::filesystem::is_empty(results_csv_file)) {
        out_csv << "map_file;scen_file;cost;elapsed_time;obs_paths;agents_paths" << std::endl << std::flush;
    }
    out_csv << std::fixed << std::setprecision(3) << std::flush;

    shared_ptr<Scenario> scenario = read_scenario(map_file, scenario_file);

    if (v_level) {
        std::cout << "Map: " << map_file << std::endl;
        std::cout << "Solver: " << solver_name << std::endl;
        std::cout << "Time limit: " << time_limit << " sec" << std::endl;
        std::cout << "Scenario: " << scenario_file << std::endl;
        std::cout << *scenario << std::endl;
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << std::filesystem::path(map_file).filename().string() << ";";
    ss << std::filesystem::path(scenario_file).filename().string() << ";";

    bool verbose = v_level > 1;
    bool time_limit_reached = false;
    Timer timer;
    timer.reset();
    shared_ptr<Plan> plan =
            (solver_name == "NATCBS") ? NATCBS(*scenario, verbose).solve(time_limit, time_limit_reached) :
            nullptr;
    timer.stop();
    auto elapsed_seconds = timer.elapsed_seconds();

    if (!plan) {

        if (time_limit_reached) {
            ss << "TLR";
            std::cout << "Time Limit Reached" << std::endl;
        } else {
            ss << "NF";
            std::cout << "Not Found" << std::endl;
        }
        ss << ";" << elapsed_seconds << ";;" << std::endl;
        out_csv << ss.str() << std::flush;
        return -1;
    }

    auto valid = plan->validate_plan(scenario->map_rows, scenario->map_cols, scenario->map,
                                     scenario->agent_start_locations, scenario->pickup_locations,
                                     scenario->delivery_locations);

    if (!valid) {
        ss << "IVP;" << elapsed_seconds << ";;" << std::endl;
        std::cerr << "Invalid Plan" << std::endl;
        return -1;
    }
    if (v_level) {
        std::cout << std::fixed << std::setprecision(3);
        std::cout << *plan << "Elapsed time: " << elapsed_seconds << " sec" << std::endl << std::endl;
    }

    ss << plan->get_makespan() << ";" << elapsed_seconds << ";";
    ss << "[";
    int num_obs = static_cast<int>(plan->obs_paths.size());
    for (int i = 0; i < num_obs; ++i) {
        ss << *plan->obs_paths.at(i);
        if (i < num_obs - 1) ss << ",";
    }
    ss << "];[";
    for (int i = 0; i < scenario->num_of_agents; ++i) {
        ss << *plan->agent_paths.at(i);
        if (i < scenario->num_of_agents - 1) ss << ",";
    }
    ss << "]" << std::endl;

    string out_str = ss.str();
    out_csv << out_str << std::flush;
    if (v_level == 0) {
        std::cout << out_str;
    }

    out_csv.close();
    return 0;
}
