#ifndef EDGE
#define EDGE

#include<optional>

#include"types.hpp"
#include"label_type.hpp"

class state_tracker;
class node;
class node_rating;

class edge{
        label_type label;
        std::optional<uint> target = std::nullopt;
    public:
        edge(void)=delete;
        edge(const edge&)=delete;
        edge(edge&&)=default;
        edge& operator=(const edge&)=delete;
        edge& operator=(edge&&)=default;
        ~edge(void)=default;
        edge(const label_type& label);
        edge clone_edge(std::vector<node>& new_nodes_register, const state_tracker& tracker)const;
        void create_target(state_tracker& tracker);
        node& get_target(state_tracker& tracker);
        const node& get_target(const state_tracker& tracker)const;
        priority get_priority(const node_rating& parent_rating, const state_tracker& tracker)const;
        double average_score(const state_tracker& tracker)const;
        const label_type& get_label(void)const;
};

#endif
