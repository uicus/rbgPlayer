#ifndef NODE
#define NODE

#include<vector>
#include<optional>

#include"types.hpp"
#include"node_address.hpp"
#include"node_rating.hpp"
#include"simulation_result.hpp"
#include"edge.hpp"

class state_tracker;

enum node_status{
    unexplored, // is it even possible?
    simulation_ongoing,
    deadend,
    nondeadend
};

enum terminal_status{
    unknown,
    terminal,
    nonterminal
};

class node{
        std::optional<std::vector<edge>> children = {};
        node_rating rating = {};
        node_status status : 2;
        terminal_status tstatus : 2;
        const node& get_node_by_address(const node_address& address, uint current_address_position, state_tracker& tracker);
        void choose_unexplored_state_for_simulation(node_address& current_address, state_tracker& tracker);
        bool choose_nodal_state_for_simulation(node_address& current_address, state_tracker& tracker);
        uint children_with_highest_priority(const state_tracker& tracker)const;
        void apply_simulation_result_for_address(const simulation_result& result,
                                                 const node_address& address,
                                                 uint current_address_position,
                                                 state_tracker& tracker);
        void choose_best_move(reasoner::move& move_so_far, state_tracker& tracker);
        std::vector<std::tuple<double,uint>> create_list_of_semichildren_for_move(state_tracker& tracker)const;
        void get_node_address_by_move(const reasoner::move& m, state_tracker& tracker, uint current_move_position, node_address& address_so_far);
        std::tuple<uint, uint> get_child_match(const reasoner::move& m, uint current_move_position)const;
        std::tuple<std::vector<std::tuple<double,uint>>,std::vector<std::tuple<double,uint>>> split_children_by_exploration(state_tracker& tracker);
    public:
        node(void);
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node clone_node(std::vector<node>& new_nodes_register, const state_tracker& tracker)const;
        const node& get_node_by_address(const node_address& address, state_tracker& tracker);
        const reasoner::move choose_best_move(state_tracker& tracker);
        node_address choose_state_for_simulation(state_tracker& tracker);
        void apply_simulation_result_for_address(const simulation_result& result, const node_address& address, state_tracker& tracker);
        node_address get_node_address_by_move(const reasoner::move& m, state_tracker& tracker);
        bool is_terminal(state_tracker& tracker);
        const node_rating& get_rating(void)const;
};

#endif
