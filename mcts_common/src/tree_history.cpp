#include"tree_history.hpp"
#include"simulation_response.hpp"

uint tree_history::game_turn(void)const{
    return past_moves.size();
}

bool tree_history::address_fully_usable(const simulation_response& response)const{
    return response.game_turn == game_turn();
}

bool tree_history::address_partially_usable(const simulation_response& response)const{
    uint lag = game_turn() - response.game_turn;
    if(lag > response.address.size())
        return false;
    for(uint i=0;i<lag;++i)
        if(past_moves[response.game_turn+i] != response.address[i])
            return false;
    return true;
}

node_address tree_history::extract_usable_address(const simulation_response& response)const{
    uint lag = game_turn() - response.game_turn;
    return node_address(response.address.begin()+lag,response.address.end());
}

void tree_history::notify_about_move(const node_address& address){
    past_moves.insert(past_moves.end(), address.begin(), address.end());
}
