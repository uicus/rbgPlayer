#include"node_rating.hpp"
#include"constants.hpp"
#include<cmath>

double node_rating::exploration_value(uint parent_simulations)const{
    return EXPLORATION_CONSTANT*sqrt(log(double(parent_simulations))/double(number_of_attempts));
}

double node_rating::average_score(uint player)const{
    if(number_of_simulations == 0)
        return EXPECTED_MAX_SCORE;
    else
        return sum_of_scores.get_player_score_divided_by(player, number_of_simulations);
}

priority node_rating::get_priority(const node_rating& parent_rating, uint parent_player)const{
    if(number_of_attempts == 0)
        return INF;
    else
        return average_score(parent_player)/EXPECTED_MAX_SCORE+exploration_value(parent_rating.number_of_simulations);
}

bool node_rating::ever_visited(void)const{
    return number_of_attempts > 0;
}

bool node_rating::ever_succeeded_simulation(void)const{
    return number_of_simulations > 0;
}

void node_rating::apply_simulation_result(const result_container& result){
    sum_of_scores += result;
    ++number_of_simulations;
}

void node_rating::apply_simulation_trial(void){
    ++number_of_attempts;
}
