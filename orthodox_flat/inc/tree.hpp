#ifndef TREE
#define TREE

#include<vector>
#include<random>

#include"reasoner.hpp"
#include"types.hpp"
#include"node_address.hpp"
#include"node.hpp"

class simulation_result;

class tree{
        reasoner::game_state root_state;
        std::mt19937 random_numbers_generator;
        std::vector<node> children = {};
        reasoner::resettable_bitarray_stack cache = {};
        void go_to_completion(reasoner::game_state& state);
        void create_children(void);
        uint get_current_player(void)const;
        uint get_node_index_by_move(const reasoner::move& m)const;
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
        bool should_simulate(uint own_index)const;
};

#endif
