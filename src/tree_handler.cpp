#include"tree_handler.hpp"
#include"tree.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"
#include"constants.hpp"

tree_handler::tree_handler(const reasoner::game_state& initial_state, concurrent_queue<simulation_request>& requests)
  : t(initial_state)
  , requests(requests){
    create_more_requests();
}

void tree_handler::create_more_requests(){
    uint created_requests_so_far = 0;
    while(requests.size() < MIN_REQUESTS_IN_CHANNEL and created_requests_so_far < MAX_NEW_REQUESTS_PER_ITERATION){
        auto [address, state] = t.choose_state_for_simulation();
        requests.emplace_back(simulation_request{std::move(state), std::move(address), game_turn});
        ++created_requests_so_far;
    }
}

void tree_handler::handle_simulation_response(const simulation_response& response){
    if(response.game_turn == game_turn){ // TODO: older responses can be exploited sometimes
        t.apply_simulation_result(response.address, response.result);
        create_more_requests();
    }
}
