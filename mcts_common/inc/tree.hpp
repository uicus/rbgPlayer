#ifndef TREE
#define TREE

#include<vector>
#include<random>

#include"reasoner.hpp"
#include"types.hpp"
#include"node.hpp"
#include"simulation_result.hpp"
#include"label_type.hpp"

class tree{
        std::vector<label_type> children_label_container = {};
        std::vector<node> nodes_register = {};
        uint root_index = 0;
        reasoner::resettable_bitarray_stack cache = {};
        std::mt19937 random_numbers_generator;
        reasoner::game_state root_state;
        void mitigate_pointers_invalidation_during_expansion(void);
        void mitigate_pointers_invalidation_during_reparentng(std::vector<node>& new_nodes_register)const;
        state_tracker create_tracker(void);
    public:
        tree(void)=delete;
        tree(const tree&)=delete;
        tree(tree&&)=default;
        tree& operator=(const tree&)=delete;
        tree& operator=(tree&&)=default;
        ~tree(void)=default;
        tree(const reasoner::game_state& initial_state);
        void apply_simulation_result(const node_address& address, const simulation_result& result);
        std::tuple<node_address, reasoner::game_state> choose_state_for_simulation(void);
        node_address reparent_along_move(const reasoner::move& m);
        reasoner::move choose_best_move(void);
        game_status_indication get_status(uint own_index);
        bool should_simulate(uint own_index);
};

#endif
