#ifndef NODE
#define NODE

#include<vector>
#include<optional>
#include<random>

#include"types.hpp"
#include"node_address.hpp"
#include"node_rating.hpp"
#include"simulation_result.hpp"
#include"edge.hpp"

class state_tracker;

class node{
        std::optional<std::vector<edge>> children = {};
        node_rating rating;
        const node& get_node_by_address(const node_address& address, uint current_address_position, state_tracker& tracker);
        void choose_state_for_simulation(node_address& current_address, state_tracker& tracker);
        uint children_with_highest_priority(const state_tracker& tracker)const;
        void apply_simulation_result_for_address(const simulation_result& result,
                                                 const node_address& address,
                                                 uint current_address_position,
                                                 state_tracker& tracker);
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        node(std::mt19937& random_numbers_generator);
        ~node(void)=default;
        node clone_node(std::vector<node>& new_nodes_register, const state_tracker& tracker)const;
        const node& get_node_by_address(const node_address& address, state_tracker& tracker);
        const reasoner::move& choose_best_move(const state_tracker& tracker);
        node_address choose_state_for_simulation(state_tracker& tracker);
        void apply_simulation_result_for_address(const simulation_result& result, const node_address& address, state_tracker& tracker);
        node_address get_node_address_by_move(const reasoner::move& m, state_tracker& tracker);
        bool is_terminal(state_tracker& tracker);
        const node_rating& get_rating(void)const;
};

#endif
