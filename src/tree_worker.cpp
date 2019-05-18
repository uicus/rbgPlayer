#include"tree_worker.hpp"
#include"tree_handler.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"

void run_tree_worker(const reasoner::game_state& initial_state,
                     concurrent_queue<simulation_request>& requests,
                     concurrent_queue<simulation_response>& responses){
    tree_handler th(initial_state, requests);
    while(true){
        const auto response = responses.pop_front();
        th.handle_simulation_response(response);
    }
}
