#include"tree_worker.hpp"
#include"tree_handler.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"
#include"client_response.hpp"

void run_tree_worker(const reasoner::game_state& initial_state,
                     const std::string& own_player_name,
                     concurrent_queue<simulation_request>& requests_to_workers,
                     concurrent_queue<client_response>& responses_to_server,
                     concurrent_queue<simulation_response>& responses){
    tree_handler th(initial_state, own_player_name, requests_to_workers, responses_to_server);
    while(true){
        const auto response = responses.pop_front();
        th.handle_simulation_response(response);
    }
}
