#ifndef NODE
#define NODE

#include"types.hpp"
#include"simulation_result.hpp"
#include"reasoner.hpp"
#include<random>

class node{
        reasoner::game_state state;
        reasoner::move move;
        double random_score_offset = 0.0;
        uint number_of_simulations = 0;
        simulation_result sum_of_scores = {};
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node(reasoner::game_state&& state, reasoner::move&& move, std::mt19937& random_numbers_generator);
        void apply_simulation_result(const simulation_result& result);
        const reasoner::game_state& get_state(void)const;
        const reasoner::move& get_move(void)const;
        double average_score(uint player)const;
        bool matches(const reasoner::move& m)const;
};

#endif
