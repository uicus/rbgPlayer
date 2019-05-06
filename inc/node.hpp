#ifndef NODE
#define NODE

#include<vector>
#include<random>

#include"edge.hpp"
#include"reasoner.hpp"
#include"types.hpp"
#include"simulation_result.hpp"

class node{
        reasoner::resettable_bitarray_stack& cache;
        reasoner::game_state state;
        std::vector<edge> children = {};
        bool terminal = false;
        uint number_of_simulations = 0;
        simulation_result sum_of_scores = {};
        void go_to_completion(void);
        double average_score(uint player)const;
        double average_current_player_score(void)const;
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state);
        void expand_children(void);
        simulation_result perform_simulation(std::mt19937& mt)const;
        void apply_simulation_result(const simulation_result& result);
};

#endif
