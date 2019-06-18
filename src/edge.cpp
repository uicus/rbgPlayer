#include"edge.hpp"
#include"node.hpp"
#include"constants.hpp"

edge::edge(const reasoner::move& label, std::vector<node>& nodes_register)
  : label(label)
  , nodes_register(nodes_register){}

edge edge::clone_edge(std::vector<node>& new_nodes_register)const{
    edge result(label, new_nodes_register);
    result.target = target;
    if(target)
        new_nodes_register.emplace_back(nodes_register[*target].clone_node(new_nodes_register));
    return result;
}

void edge::create_target(const node& source_node){
    if(not target){
        nodes_register.emplace_back(source_node.create_node_after_move(label));
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
        return INF;
    else
        return nodes_register[*target].get_priority(parent_simulations, parent_player);
}

bool edge::matches(const reasoner::move& m)const{
    return label == m;
}

double edge::average_score(uint player)const{
    if(not target)
        return 0.0;
    else
        return nodes_register[*target].average_score(player);
}

const reasoner::move& edge::get_move(void)const{
    return label;
}
