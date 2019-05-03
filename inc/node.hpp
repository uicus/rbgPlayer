#ifndef NODE
#define NODE

#include<vector>

#include"edge.hpp"
#include"reasoner.hpp"

class node{
        reasoner::resettable_bitarray_stack& cache;
        reasoner::game_state state;
        std::vector<edge> children;
        bool terminal;
        void go_to_completion(void);
    public:
        node(void)=delete;
        node(const node&)=delete;
        node(node&&)=default;
        node& operator=(const node&)=delete;
        node& operator=(node&&)=default;
        ~node(void)=default;
        node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state);
        void expand_children(void);
};

#endif
