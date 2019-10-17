#include"node.hpp"
#include"reasoner.hpp"
#include"constants.hpp"

node::node(reasoner::game_state&& state, reasoner::move&& move)
  : state(std::move(state))
  , move(std::move(move)){
}

void node::apply_simulation_result(const simulation_result& result){
    sum_of_scores += result;
    ++number_of_simulations;
}

const reasoner::game_state& node::get_state(void)const{
    return state;
}

const reasoner::move& node::get_move(void)const{
    return move;
}

double node::average_score(uint player)const{
    if(number_of_simulations == 0)
        return EXPECTED_MAX_SCORE;
    else
        return sum_of_scores.get_player_score_divided_by(player, number_of_simulations);
}

bool node::matches(const reasoner::move& m)const{
    return move == m;
}
