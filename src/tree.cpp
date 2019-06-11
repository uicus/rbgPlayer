#include"tree.hpp"
#include"edge.hpp"
#include"constants.hpp"

tree::tree(const reasoner::game_state& initial_state){
    node root(cache, initial_state, nodes_register);
    nodes_register.emplace_back(std::move(root));
}

void tree::mitigate_pointers_invalidation_during_expansion(void){
    if(nodes_register.capacity()<nodes_register.size()+MAX_TRIES_FOR_NON_TERMINAL_STATE)
        nodes_register.reserve(2*nodes_register.size()+MAX_TRIES_FOR_NON_TERMINAL_STATE);
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

uint tree::reparent_along_move(const reasoner::move& m){
    std::vector<node> new_nodes_register;
    mitigate_pointers_invalidation_during_reparentng(new_nodes_register);
    const auto move_index = nodes_register[root_index].get_node_index_by_move(m);
    const auto& new_root = nodes_register[root_index].get_node_by_address({move_index});
    new_nodes_register.emplace_back(new_root.clone_node(new_nodes_register));
    root_index = new_nodes_register.size()-1;
    std::swap(new_nodes_register, nodes_register);
    return move_index;
}

const reasoner::move& tree::choose_best_move(void){
    return nodes_register[root_index].choose_best_move();
}

game_status_indication tree::get_status(uint own_index)const{
    if(nodes_register[root_index].is_terminal())
        return end_game;
    else if(nodes_register[root_index].get_current_player() == own_index)
        return own_turn;
    else
        return opponent_turn;
}
