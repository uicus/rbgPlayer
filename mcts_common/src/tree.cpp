#include"tree.hpp"
#include"edge.hpp"
#include"constants.hpp"
#include"state_tracker.hpp"

tree::tree(const reasoner::game_state& initial_state)
  : random_numbers_generator()
  , root_state(initial_state){
    std::random_device d;
    random_numbers_generator.seed(d());
    auto tracker = create_tracker();
    tracker.go_to_completion();
    root_state = tracker.get_state();
    root_index = tracker.add_node_to_register();
}

void tree::mitigate_pointers_invalidation_during_expansion(void){
    if(nodes_register.capacity()<nodes_register.size()+MAX_TRIES_FOR_NON_TERMINAL_STATE)
        nodes_register.reserve(2*nodes_register.capacity()+MAX_TRIES_FOR_NON_TERMINAL_STATE);
}

void tree::mitigate_pointers_invalidation_during_reparentng(std::vector<node>& new_nodes_register)const{
    new_nodes_register.reserve(nodes_register.capacity());
}

state_tracker tree::create_tracker(void){
    return state_tracker(cache, children_label_container, nodes_register, random_numbers_generator, root_state);
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    auto tracker = create_tracker();
    nodes_register[root_index].apply_simulation_result_for_address(result, address, tracker);
}

std::tuple<node_address, reasoner::game_state> tree::choose_state_for_simulation(void){
    mitigate_pointers_invalidation_during_expansion();
    auto tracker = create_tracker();
    auto address = nodes_register[root_index].choose_state_for_simulation(tracker);
    return {std::move(address), tracker.get_state()};
}

node_address tree::reparent_along_move(const reasoner::move& m){
    std::vector<node> new_nodes_register;
    auto address_getter_tracker = create_tracker();
    mitigate_pointers_invalidation_during_expansion();
    const auto move_address = nodes_register[root_index].get_node_address_by_move(m, address_getter_tracker);
    auto tracker = create_tracker();
    const auto& new_root = nodes_register[root_index].get_node_by_address(move_address, tracker);
    root_state = tracker.get_state();
    mitigate_pointers_invalidation_during_reparentng(new_nodes_register);
    auto&& cloned_root_node = new_root.clone_node(new_nodes_register, tracker);
    new_nodes_register.emplace_back(std::move(cloned_root_node));
    root_index = new_nodes_register.size()-1;
    std::swap(new_nodes_register, nodes_register);
    return move_address;
}

reasoner::move tree::choose_best_move(void){
    auto tracker = create_tracker();
    return nodes_register[root_index].choose_best_move(tracker);
}

game_status_indication tree::get_status(uint own_index){
    auto tracker = create_tracker();
    if(nodes_register[root_index].is_terminal(tracker))
        return end_game;
    else if(uint(root_state.get_current_player())-1 == own_index)
        return own_turn;
    else
        return opponent_turn;
}

bool tree::should_simulate(uint own_index){
    return get_status(own_index) == own_turn;
    // return true;
}
