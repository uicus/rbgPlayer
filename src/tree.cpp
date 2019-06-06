#include"tree.hpp"
#include"edge.hpp"

tree::tree(const reasoner::game_state& initial_state){
    node root(cache, initial_state, nodes_register);
    nodes_register.emplace_back(std::move(root));
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    nodes_register[root_index].apply_simulation_result_for_address(result, address);
}

std::optional<std::tuple<node_address, const reasoner::game_state&>> tree::choose_state_for_simulation(void){
    return nodes_register[root_index].choose_state_for_simulation();
}
