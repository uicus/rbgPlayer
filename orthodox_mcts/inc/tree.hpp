#ifndef TREE
#define TREE

#include<vector>

#include"reasoner.hpp"
#include"types.hpp"
#include"node.hpp"

class simulation_result;

class tree{
        std::vector<node> nodes_register = {};
        uint root_index = 0;
        reasoner::resettable_bitarray_stack cache = {};
        reasoner::game_state root_state;
        void mitigate_pointers_invalidation_during_expansion(void);
        void mitigate_pointers_invalidation_during_reparentng(std::vector<node>& new_nodes_register)const;
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
        uint reparent_along_move(const reasoner::move& m);
        const reasoner::move& choose_best_move(void);
        game_status_indication get_status(uint own_index);
        bool should_simulate(uint own_index);
};

#endif
