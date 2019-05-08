#include"edge.hpp"
#include"node.hpp"

edge::edge(const reasoner::move& label, const node& parent)
  : label(label)
  , parent(parent){}

void edge::create_target(void){
    if(not target)
        target = std::unique_ptr<node>(new node(parent.create_node_after_move(label)));
}

const node& edge::get_target(void)const{
    return *target;
}

node& edge::get_target(void){
    return *target;
}

double edge::get_priority(uint parent_simulations, uint parent_player)const{
    if(not target)
        return INFINITY;
    else
        return target->get_priority(parent_simulations, parent_player);
}
