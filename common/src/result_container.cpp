#include"result_container.hpp"

result_container::result_container(const reasoner::game_state& state){
    for (uint i = 1; i < reasoner::NUMBER_OF_PLAYERS; ++i)
        scores[i-1] = state.get_player_score(i);
}

result_container& result_container::operator+=(const result_container& rhs){
    for (uint i = 1; i < reasoner::NUMBER_OF_PLAYERS; ++i)
        scores[i-1] += rhs.scores[i-1];
    return *this;
}

double result_container::get_player_score_divided_by(uint player, uint denominator)const{
    return double(scores[player])/double(denominator);
}
