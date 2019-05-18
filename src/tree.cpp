#include"tree.hpp"
#include"edge.hpp"

tree::tree(const reasoner::game_state& initial_state){
    node root(cache, initial_state, nodes_register);
    nodes_register.emplace_back(std::move(root));
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    for(const auto el: address)
        nodes_register[el].apply_simulation_result(result);
}

std::tuple<node_address, const reasoner::game_state&> tree::choose_state_for_simulation(void){
    return nodes_register[root_index].choose_state_for_simulation(root_index);
}
