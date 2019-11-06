#ifndef RESULT_CONTAINER
#define RESULT_CONTAINER

#include<vector>

#include"reasoner.hpp"
#include"types.hpp"

class result_container{
        uint scores[reasoner::NUMBER_OF_PLAYERS-1] = {};
    public:
        result_container(void)=default;
        result_container(const result_container&)=default;
        result_container(result_container&&)=default;
        result_container& operator=(const result_container&)=default;
        result_container& operator=(result_container&&)=default;
        ~result_container(void)=default;
        result_container(const reasoner::game_state& state);
        result_container& operator+=(const result_container& rhs);
        double get_player_score_divided_by(uint player, uint denominator)const;
};

#endif
