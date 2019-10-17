#ifndef NODE
#define NODE

#include<vector>
#include<optional>

#include"types.hpp"
#include"node_address.hpp"
#include"simulation_result.hpp"
#include"edge.hpp"

class state_tracker;

class node{
        std::optional<std::vector<edge>> children = {};
        uint number_of_simulations = 0;
        uint number_of_attempts = 0;
        simulation_result sum_of_scores = {};
        double exploration_value(uint parent_simulations)const;
        const node& get_node_by_address(const node_address& address, uint current_address_position, state_tracker& tracker);
        void choose_state_for_simulation(node_address& current_address, state_tracker& tracker);
        uint children_with_highest_priority(const state_tracker& tracker)const;
        void apply_simulation_result(const simulation_result& result);
        void apply_simulation_result_for_address(const simulation_result& result,
                                                 const node_address& address,
                                                 uint current_address_position,
                                                 state_tracker& tracker);
    public:
        node(void)=default;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node clone_node(std::vector<node>& new_nodes_register, const state_tracker& tracker)const;
        const node& get_node_by_address(const node_address& address, state_tracker& tracker);
        priority get_priority(uint parent_simulations, uint parent_player)const;
        double average_score(uint player)const;
        const reasoner::move& choose_best_move(const state_tracker& tracker);
        node_address choose_state_for_simulation(state_tracker& tracker);
        void apply_simulation_result_for_address(const simulation_result& result, const node_address& address, state_tracker& tracker);
        uint get_node_index_by_move(const reasoner::move& m)const;
        bool is_terminal(void)const;
};

#endif
