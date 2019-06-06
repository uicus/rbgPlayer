#include"edge.hpp"
#include"node.hpp"
#include"constants.hpp"

edge::edge(const reasoner::move& label, const node& parent, std::vector<node>& nodes_register)
  : label(label)
  , parent(parent)
  , nodes_register(nodes_register){}

void edge::create_target(void){
    if(not target){
        nodes_register.emplace_back(parent.create_node_after_move(label));
        target = nodes_register.size() - 1;
    }
}

const node& edge::get_target(void)const{
    assert(target);
    return nodes_register[*target];
}

node& edge::get_target(void){
    assert(target);
    return nodes_register[*target];
}

priority edge::get_priority(uint parent_simulations, uint parent_player)const{
    if(not target)
        return {INF,0};
    else
        return nodes_register[*target].get_priority(parent_simulations, parent_player);
}
