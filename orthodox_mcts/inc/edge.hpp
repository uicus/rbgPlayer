#ifndef EDGE
#define EDGE

#include<optional>

#include"reasoner.hpp"
#include"types.hpp"

class state_tracker;
class node;

class edge{
        reasoner::move label;
        std::optional<uint> target = std::nullopt;
    public:
        edge(void)=delete;
        edge(const edge&)=delete;
        edge(edge&&)=default;
        edge& operator=(const edge&)=delete;
        edge& operator=(edge&&)=default;
        ~edge(void)=default;
        edge(const reasoner::move& label);
        edge clone_edge(std::vector<node>& new_nodes_register, const state_tracker& tracker)const;
        void create_target(state_tracker& tracker);
        node& get_target(state_tracker& tracker);
        const node& get_target(const state_tracker& tracker)const;
        priority get_priority(uint parent_simulations, const state_tracker& tracker)const;
        bool matches(const reasoner::move& m)const;
        double average_score(const state_tracker& tracker)const;
        const reasoner::move& get_move(void)const;
};

#endif
