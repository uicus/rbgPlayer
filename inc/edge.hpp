#ifndef EDGE
#define EDGE

#include<vector>
#include<optional>

#include"reasoner.hpp"
#include"types.hpp"

class node;

class edge{
        reasoner::move label;
        const node& parent;
        std::vector<node>& nodes_register;
        std::optional<uint> target = std::nullopt;
    public:
        edge(void)=delete;
        edge(const edge&)=delete;
        edge(edge&&)=default;
        edge& operator=(const edge&)=delete;
        edge& operator=(edge&&)=default;
        ~edge(void)=default;
        edge(const reasoner::move& label, const node& parent, std::vector<node>& nodes_register);
        void create_target(void);
        node& get_target(void);
        const node& get_target(void)const;
        priority get_priority(uint parent_simulations, uint parent_player)const;
};

#endif
