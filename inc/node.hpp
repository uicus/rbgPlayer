#ifndef NODE
#define NODE

#include<vector>
#include<optional>

#include"reasoner.hpp"
#include"types.hpp"
#include"simulation_result.hpp"
#include"edge.hpp"

class node{
        reasoner::resettable_bitarray_stack& cache;
        std::vector<node>& nodes_register;
        reasoner::game_state state;
        std::optional<std::vector<edge>> children = {};
        uint number_of_simulations = 0;
        uint number_of_attempts = 0;
        simulation_result sum_of_scores = {};
        void go_to_completion(void);
        double average_score(uint player)const;
        double exploration_value(uint parent_simulations)const;
        void expand_children(void);
        const node& get_node_by_address(const node_address& address, uint current_address_position)const;
        const reasoner::game_state& get_state(void)const;
        std::tuple<const reasoner::game_state&, bool> choose_state_for_simulation(node_address& current_address);
        uint children_with_highest_priority(void)const;
        void apply_simulation_result(const simulation_result& result);
        void apply_simulation_result_for_address(const simulation_result& result, const node_address& address, uint current_address_position);
        node(reasoner::resettable_bitarray_stack& cache, reasoner::game_state&& state, std::vector<node>& nodes_register);
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state, std::vector<node>& nodes_register);
        node create_node_after_move(const reasoner::move& m)const;
        const node& get_node_by_address(const node_address& address)const;
        priority get_priority(uint parent_simulations, uint parent_player)const;
        std::optional<std::tuple<node_address, const reasoner::game_state&>> choose_state_for_simulation(void);
        void apply_simulation_result_for_address(const simulation_result& result, const node_address& address);
};

#endif
