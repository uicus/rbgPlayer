#include"tree_history.hpp"
#include"simulation_response.hpp"

uint tree_history::game_turn(void)const{
    return current_turn;
}

bool tree_history::address_fully_usable(const simulation_response& response)const{
    return response.game_turn == game_turn();
}

bool tree_history::address_partially_usable(const simulation_response& response)const{
    return address_fully_usable(response);
}

node_address tree_history::extract_usable_address(const simulation_response& response)const{
    return response.address;
}

void tree_history::notify_about_move(node_address){
    ++current_turn;
}
