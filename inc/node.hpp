#ifndef NODE
#define NODE

#include<vector>
#include<random>

#include"edge.hpp"
#include"reasoner.hpp"
#include"types.hpp"
#include"simulation_result.hpp"

typedef std::vector<uint> node_address;

class node{
        reasoner::resettable_bitarray_stack& cache;
        reasoner::game_state state;
        std::vector<edge> children = {};
        bool children_already_expanded = false;
        uint number_of_simulations = 0;
        uint number_of_attempts = 0;
        simulation_result sum_of_scores = {};
        void go_to_completion(void);
        double average_score(uint player)const;
        double exploration_value(uint parent_simulations)const;
        void expand_children(void);
        const node& get_node_by_address(const node_address& address, uint current_address_position)const;
        void apply_simulation_result(const simulation_result& result);
        void apply_simulation_result_by_address(const simulation_result& result,
                                                const node_address& address,
                                                uint current_address_position);
        const reasoner::game_state& get_state(void)const;
        const reasoner::game_state& choose_state_for_simulation(node_address& current_address);
        uint children_with_highest_priority(void)const;
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state);
        node(reasoner::resettable_bitarray_stack& cache, reasoner::game_state&& state);
        node create_node_after_move(const reasoner::move& m)const;
        const node& get_node_by_address(const node_address& address)const;
        void apply_simulation_result_by_address(const simulation_result& result,
                                                const node_address& address);
        double get_priority(uint parent_simulations, uint parent_player)const;
        std::tuple<node_address, const reasoner::game_state&> choose_state_for_simulation(void);
};

#endif
