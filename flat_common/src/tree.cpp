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
    root_state.get_all_moves(cache, legal_moves);
    std::transform(legal_moves.begin(), legal_moves.end(), std::back_inserter(children),
        [this](auto&& move){
            auto child_state = root_state;
            child_state.apply_move(move);
            go_to_completion(child_state);
            return node(std::move(child_state), std::move(move), random_numbers_generator);
        });
}

uint tree::get_current_player(void)const{
    return root_state.get_current_player()-1;
}

uint tree::get_node_index_by_move(const reasoner::move& m)const{
    const auto result = std::find_if(children.begin(), children.end(), [&m](const auto& el){return el.matches(m);});
    assert(result != children.end()); // told to move along nonexistant edge -- probably server bug
    return std::distance(children.begin(), result);
}

void tree::apply_simulation_result(const node_address& address, const simulation_result& result){
    children[address].apply_simulation_result(result);
}

std::tuple<node_address, reasoner::game_state> tree::choose_state_for_simulation(void){
    std::uniform_int_distribution<> random_distribution(0, children.size()-1);
    uint chosen_child = random_distribution(random_numbers_generator);
    return {chosen_child, children[chosen_child].get_state()};
}

node_address tree::reparent_along_move(const reasoner::move& m){
    node_address child_index = get_node_index_by_move(m);
    root_state = children[child_index].get_state();
    create_children();
    return child_index;
}

const reasoner::move& tree::choose_best_move(void){
    std::vector<double> children_to_choose;
    const auto player = get_current_player();
    std::transform(children.begin(), children.end(), std::back_inserter(children_to_choose),
        [player](const auto& el){return el.average_score(player);});
    const auto chosen_child = std::max_element(children_to_choose.begin(), children_to_choose.end());
    const auto choice_number = std::distance(children_to_choose.begin(), chosen_child);
    return children[choice_number].get_move();
}

game_status_indication tree::get_status(uint own_index)const{
    if(children.empty())
        return end_game;
    else if(get_current_player() == own_index)
        return own_turn;
    else
        return opponent_turn;
}

bool tree::should_simulate(uint own_index)const{
    return get_status(own_index) == own_turn;
}
