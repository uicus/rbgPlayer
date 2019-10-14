#ifndef SIMULATION_RESULT
#define SIMULATION_RESULT

#include<vector>

#include"reasoner.hpp"
#include"types.hpp"

class simulation_result{
        uint scores[reasoner::NUMBER_OF_PLAYERS-1] = {};
    public:
        simulation_result(void)=default;
        simulation_result(const simulation_result&)=default;
        simulation_result(simulation_result&&)=default;
        simulation_result& operator=(const simulation_result&)=default;
        simulation_result& operator=(simulation_result&&)=default;
        ~simulation_result(void)=default;
        simulation_result(const reasoner::game_state& state);
        simulation_result& operator+=(const simulation_result& rhs);
        double get_player_score_divided_by(uint player, uint denominator)const;
};

#endif
