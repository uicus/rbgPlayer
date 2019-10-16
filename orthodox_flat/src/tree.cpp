#include"tree.hpp"
#include<algorithm>
#include"constants.hpp"

tree::tree(const reasoner::game_state& initial_state)
  : root_state(initial_state)
  , random_numbers_generator(){
    go_to_completion(root_state);
    std::random_device d;
    random_numbers_generator.seed(d());
    create_children();
}

void tree::go_to_completion(reasoner::game_state& state){
    while(state.get_current_player() == KEEPER and state.apply_any_move(cache));
}

void tree::create_children(void){
    children.clear();
    auto legal_moves = root_state.get_all_moves(cache);
    std::transform(legal_moves.begin(), legal_moves.end(), std::back_inserter(children),
        [this](const auto& move){
            auto child_state = root_state;
            child_state.apply_move(move);
            go_to_completion(child_state);
            return node(std::move(child_state));
        });
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    children[address].apply_simulation_result(result);
}

std::tuple<node_address, reasoner::game_state> tree::choose_state_for_simulation(void){
    std::uniform_int_distribution<> random_distribution(0, children.size()-1);
    uint chosen_child = random_distribution(random_numbers_generator);
    return {chosen_child, children[chosen_child].get_state()};
}

uint tree::reparent_along_move(const reasoner::move& m){
    assert(false); //TODO
}

const reasoner::move& tree::choose_best_move(void){
    assert(false); //TODO
}

game_status_indication tree::get_status(uint own_index)const{
    assert(false); //TODO
}
