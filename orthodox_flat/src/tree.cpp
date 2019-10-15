#include"tree.hpp"

tree::tree(const reasoner::game_state& initial_state)
  : root_state(initial_state){
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    assert(false); //TODO
}

std::tuple<node_address, reasoner::game_state> tree::choose_state_for_simulation(void){
    assert(false); //TODO
}

uint tree::reparent_along_move(const reasoner::move& m){
    assert(false); //TODO
}

const reasoner::move& tree::choose_best_move(void){
    assert(false); //TODO
}

game_status_indication tree::get_status(uint own_index)const{
    assert(false); //TODO
}
