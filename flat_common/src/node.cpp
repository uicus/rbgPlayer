#include"node.hpp"
#include"reasoner.hpp"
#include"constants.hpp"

node::node(reasoner::game_state&& state, reasoner::move&& move, std::mt19937& random_numbers_generator)
  : state(std::move(state))
  , move(std::move(move)){
    std::uniform_real_distribution<double> random_distribution(-1.0, 1.0);
    random_score_offset = random_distribution(random_numbers_generator);
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
    return (number_of_simulations == 0 ? UNEXPLORED_SCORE : sum_of_scores.get_player_score_divided_by(player, number_of_simulations)) + random_score_offset;
}

bool node::matches(const reasoner::move& m)const{
    return move == m;
}
