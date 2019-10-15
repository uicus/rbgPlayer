#ifndef TREE
#define TREE

#include<vector>

#include"reasoner.hpp"
#include"types.hpp"
#include"node_address.hpp"

class simulation_result;

class tree{
        reasoner::game_state root_state;
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
        game_status_indication get_status(uint own_index)const;
};

#endif
