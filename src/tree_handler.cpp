#include"tree_handler.hpp"
#include"tree.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"
#include"client_response.hpp"
#include"constants.hpp"

namespace{
uint get_player_index(const std::string& name){
    for(uint i=0;i<reasoner::NUMBER_OF_PLAYERS-1;++i)
        if(reasoner::variable_to_string(i) == name)
            return i;
    assert(false); // no such player?
}
}

tree_handler::tree_handler(const reasoner::game_state& initial_state,
                           const std::string& own_player_name,
                           concurrent_queue<simulation_request>& requests_to_workers,
                           concurrent_queue<client_response>& responses_to_server)
  : t(initial_state)
  , own_player_index(get_player_index(own_player_name))
  , requests_to_workers(requests_to_workers)
  , responses_to_server(responses_to_server){
    create_more_requests();
}

void tree_handler::create_more_requests(){
    uint created_requests_so_far = 0;
    while(requests_to_workers.size() < MIN_REQUESTS_IN_CHANNEL and created_requests_so_far < MAX_NEW_REQUESTS_PER_ITERATION){
        auto find_result = t.choose_state_for_simulation();
        if(find_result){
            auto& [address, state] = *find_result;
            requests_to_workers.emplace_back(simulation_request{std::move(state), std::move(address), game_turn});
            ++created_requests_so_far;
        }
        else
            return;
    }
}

bool tree_handler::address_still_usable(const simulation_response& response)const{
    uint lag = game_turn - response.game_turn;
    for(uint i=0;i<lag;++i)
        if(moves_history[response.game_turn+i] != response.address[i])
            return false;
    return true;
}

node_address tree_handler::extract_usable_address(const simulation_response& response)const{
    uint lag = game_turn - response.game_turn;
    return node_address(response.address.begin()+lag,response.address.end());
}

void tree_handler::handle_simulation_response(const simulation_response& response){
    if(response.game_turn == game_turn)
        t.apply_simulation_result(response.address, response.result);
    else if(address_still_usable(response))
        t.apply_simulation_result(extract_usable_address(response), response.result);
    create_more_requests();
}

void tree_handler::handle_move_request(void){
    const auto& chosen_move = t.choose_best_move();
    responses_to_server.emplace_back(client_response{chosen_move});
}

void tree_handler::handle_move_indication(const reasoner::move& m){
    moves_history.emplace_back(t.reparent_along_move(m));
    responses_to_server.emplace_back(client_response{t.get_status(own_player_index)});
}
