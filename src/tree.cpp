#include"tree.hpp"
#include"edge.hpp"
#include"constants.hpp"

tree::tree(const reasoner::game_state& initial_state){
    node root(cache, initial_state, nodes_register);
    nodes_register.emplace_back(std::move(root));
}

void tree::mitigate_pointers_invalidation_during_expansion(void){
    nodes_register.reserve(nodes_register.size()+MAX_TRIES_FOR_NON_TERMINAL_STATE);
}

void tree::mitigate_pointers_invalidation_during_reparentng(std::vector<node>& new_nodes_register)const{
    new_nodes_register.reserve(nodes_register.size());
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    nodes_register[root_index].apply_simulation_result_for_address(result, address);
}

std::optional<std::tuple<node_address, const reasoner::game_state&>> tree::choose_state_for_simulation(void){
    mitigate_pointers_invalidation_during_expansion();
    return nodes_register[root_index].choose_state_for_simulation();
}

void tree::reparent_along_move(const reasoner::move& m){
    std::vector<node> new_nodes_register;
    mitigate_pointers_invalidation_during_reparentng(new_nodes_register);
    const node& new_root = new_nodes_register[root_index].move_along_the_move(m);
    new_nodes_register.emplace_back(new_root.clone_node(new_nodes_register));
    root_index = new_nodes_register.size()-1;
    std::swap(new_nodes_register, nodes_register);
}

const reasoner::move& tree::choose_best_move(void){
    return nodes_register[root_index].choose_best_move();
}
