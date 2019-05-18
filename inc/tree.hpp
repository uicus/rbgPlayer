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
    public:
        tree(void)=delete;
        tree(const tree&)=delete;
        tree(tree&&)=default;
        tree& operator=(const tree&)=delete;
        tree& operator=(tree&&)=default;
        ~tree(void)=default;
        tree(const reasoner::game_state& initial_state);
        void apply_simulation_result(const node_address& address, const simulation_result& result);
        std::tuple<node_address, const reasoner::game_state&> choose_state_for_simulation(void);
};

#endif
