#ifndef NODE
#define NODE

#include"types.hpp"
#include"simulation_result.hpp"
#include"reasoner.hpp"

class node{
        reasoner::game_state state;
        reasoner::move move;
        uint number_of_simulations = 0;
        simulation_result sum_of_scores = {};
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node(reasoner::game_state&& state, reasoner::move&& move);
        void apply_simulation_result(const simulation_result& result);
        const reasoner::game_state& get_state(void)const;
        const reasoner::move& get_move(void)const;
        double average_score(uint player)const;
        bool matches(const reasoner::move& m)const;
};

#endif
