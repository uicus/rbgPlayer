#ifndef EDGE
#define EDGE

#include<memory>

#include"reasoner.hpp"

class node;
typedef std::tuple<double,uint> priority;

class edge{
        reasoner::move label;
        const node& parent;
        std::unique_ptr<node> target;
    public:
        edge(void)=delete;
        edge(const edge&)=delete;
        edge(edge&&)=default;
        edge& operator=(const edge&)=delete;
        edge& operator=(edge&&)=default;
        ~edge(void)=default;
        edge(const reasoner::move& label, const node& parent);
        void create_target(void);
        node& get_target(void);
        const node& get_target(void)const;
        priority get_priority(uint parent_simulations, uint parent_player)const;
};

#endif
