#include"simulation_result.hpp"

simulation_result::simulation_result(const reasoner::game_state& state){
    for (uint i = 1; i < reasoner::NUMBER_OF_PLAYERS; ++i)
        scores[i-1] = state.get_player_score(i);
}

simulation_result& simulation_result::operator+=(const simulation_result& rhs){
    for (uint i = 1; i < reasoner::NUMBER_OF_PLAYERS; ++i)
        scores[i] += rhs.scores[i];
    return *this;
}

double simulation_result::get_player_score_divided_by(uint player, uint denominator)const{
    return double(scores[player])/double(denominator);
}
