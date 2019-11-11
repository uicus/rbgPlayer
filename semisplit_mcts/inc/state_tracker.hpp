#ifndef STATE_TRACKER
#define STATE_TRACKER

#include<vector>
#include"reasoner.hpp"

class node;
class edge;

class state_tracker{
        reasoner::resettable_bitarray_stack& cache;
        std::vector<node>& nodes_register;
        reasoner::game_state state;
        bool terminal = false;
    public:
        state_tracker(void)=delete;
        state_tracker(const state_tracker&)=delete;
        state_tracker(state_tracker&&)=default;
        state_tracker& operator=(const state_tracker&)=delete;
        state_tracker& operator=(state_tracker&&)=default;
        ~state_tracker(void)=default;
        state_tracker(reasoner::resettable_bitarray_stack& cache,
                      std::vector<node>& nodes_register,
                      const reasoner::game_state& state);
        void go_to_completion(void);
        std::vector<edge> generate_children(void);
        void go_along_semimove(const reasoner::semimove& m);
        uint add_node_to_register(void);
        const node& get_node(uint index)const;
        node& get_node(uint index);
        uint get_current_player(void)const;
        const reasoner::game_state& get_state(void)const;
};

#endif
