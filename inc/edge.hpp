#ifndef EDGE
#define EDGE

#include<memory>

#include"reasoner.hpp"

class node;

class edge{
        reasoner::move label;
        std::unique_ptr<node> target;
    public:
        edge(void)=delete;
        edge(const edge&)=delete;
        edge(edge&&)=default;
        edge& operator=(const edge&)=delete;
        edge& operator=(edge&&)=default;
        ~edge(void)=default;
        edge(const reasoner::move& label, std::unique_ptr<node>&& target);
};

#endif
