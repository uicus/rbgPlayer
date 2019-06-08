#ifndef EDGE
#define EDGE

#include<vector>
#include<optional>

#include"reasoner.hpp"
#include"types.hpp"

class node;

class edge{
        reasoner::move label;
        std::vector<node>& nodes_register;
        std::optional<uint> target = std::nullopt;
    public:
        edge(void)=delete;
        edge(const edge&)=delete;
        edge(edge&&)=default;
        edge& operator=(const edge&)=delete;
        edge& operator=(edge&&)=default;
        ~edge(void)=default;
        edge(const reasoner::move& label, std::vector<node>& nodes_register);
        edge clone_edge(std::vector<node>& new_nodes_register)const;
        void create_target(const node& source_node);
        node& get_target(void);
        const node& get_target(void)const;
        priority get_priority(uint parent_simulations, uint parent_player)const;
        bool matches(const reasoner::move& m)const;
        double average_score(uint player)const;
        const reasoner::move& get_move(void)const;
};

#endif
