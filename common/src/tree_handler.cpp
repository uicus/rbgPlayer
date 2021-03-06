#include"tree_handler.hpp"
#include"tree.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"
#include"client_response.hpp"
#include"constants.hpp"
#include<iostream>

namespace{
uint get_player_index(const std::string& name){
    for(uint i=0;i<reasoner::NUMBER_OF_PLAYERS-1;++i)
        if(reasoner::variable_to_string(i) == name)
            return i;
    assert(false); // no such player?
}
}

tree_handler::tree_handler(const reasoner::game_state& initial_state,
                           concurrent_queue<simulation_request>& requests_to_workers,
                           concurrent_queue<client_response>& responses_to_server)
  : t(initial_state)
  , own_player_index(get_player_index(NAME))
  , history()
  , requests_to_workers(requests_to_workers)
  , responses_to_server(responses_to_server){
    handle_status();
    create_more_requests();
}

void tree_handler::create_more_requests(){
    if(simulations_count < SIMULATIONS_PER_MOVE and t.should_simulate(own_player_index)){
        uint created_requests_so_far = 0;
        while(requests_to_workers.size() < MIN_REQUESTS_IN_CHANNEL and created_requests_so_far < MAX_NEW_REQUESTS_PER_ITERATION){
            auto&& [address, state] = t.choose_state_for_simulation();
            requests_to_workers.emplace_back(simulation_request{std::move(state), std::move(address), history.game_turn()});
            ++created_requests_so_far;
        }
    }
}

void tree_handler::handle_simulations_counter(void){
    if(++simulations_count >= SIMULATIONS_PER_MOVE and should_perform_move)
        handle_move_request();
}

void tree_handler::handle_simulation_response(const simulation_response& response){
    if(history.address_fully_usable(response)){
        t.apply_simulation_result(response.address, response.result);
        handle_simulations_counter();
    }
    else if(history.address_partially_usable(response)){
        t.apply_simulation_result(history.extract_usable_address(response), response.result);
        handle_simulations_counter();
    }
    create_more_requests();
}

void tree_handler::handle_status(void){
    auto status = t.get_status(own_player_index);
    should_perform_move = status == own_turn;
    responses_to_server.emplace_back(client_response{status});
}

void tree_handler::handle_move_request(void){
    const auto& chosen_move = t.choose_best_move();
    responses_to_server.emplace_back(client_response{chosen_move});
    std::cout << "[PLAYER] Performing move on the basis of " << simulations_count << " simulations." << std::endl;
    handle_move_indication(chosen_move);
}

void tree_handler::handle_move_indication(const reasoner::move& m){
    simulations_count = 0;
    history.notify_about_move(t.reparent_along_move(m));
    handle_status();
    create_more_requests();
}

void tree_handler::handle_reset_request(const reasoner::game_state& initial_state){
    std::cout << "[PLAYER] Reset Request!" << std::endl;
    t = initial_state;
    simulations_count = 0;
    history = {};
    handle_status();
    create_more_requests();
}
