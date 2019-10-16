#include"node.hpp"
#include"reasoner.hpp"

node::node(reasoner::game_state&& state)
  : state(std::move(state)){
}

void node::apply_simulation_result(const simulation_result& result){
    sum_of_scores += result;
    ++number_of_simulations;
}

const reasoner::game_state& node::get_state(void)const{
    return state;
}
