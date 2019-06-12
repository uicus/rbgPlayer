#include"node.hpp"
#include"constants.hpp"
#include<cmath>
#include<algorithm>

node::node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state, std::vector<node>& nodes_register)
  : cache(cache)
  , nodes_register(nodes_register)
  , state(state){
    go_to_completion();
}

node::node(reasoner::resettable_bitarray_stack& cache, reasoner::game_state&& state, std::vector<node>& nodes_register)
  : cache(cache)
  , nodes_register(nodes_register)
  , state(std::move(state)){
    go_to_completion();
}

node node::clone_node(std::vector<node>& new_nodes_register)const{
    node result(cache, state, new_nodes_register);
    result.number_of_simulations = number_of_simulations;
    result.number_of_attempts = number_of_attempts;
    result.sum_of_scores = sum_of_scores;
    result.terminal = terminal;
    if(children){
        result.children = std::vector<edge>();
        std::transform(children->begin(), children->end(), std::back_inserter(*result.children),
            [&new_nodes_register](const auto& el){return el.clone_edge(new_nodes_register);});
    }
    return result;
}

void node::go_to_completion(void){
    while(state.get_current_player() == KEEPER)
        if(not state.apply_any_move(cache)){
            terminal = true;
            break;
        }
}

double node::average_score(uint player)const{
    return sum_of_scores.get_player_score_divided_by(player, number_of_simulations);
}

double node::exploration_value(uint parent_simulations)const{
    return EXPLORATION_CONSTANT*sqrt(log(double(parent_simulations))/double(number_of_simulations));
}

void node::expand_children(void){
    if(not children){
        children = std::vector<edge>();
        if(not is_terminal()){
            auto all_moves = state.get_all_moves(cache);
            std::transform(all_moves.begin(), all_moves.end(), std::back_inserter(*children),
                [this](const auto& el){return edge(el, nodes_register);});
            terminal = children->empty();
        }
    }
}

const node& node::get_node_by_address(const node_address& address, uint current_address_position)const{
    if(current_address_position >= address.size())
        return *this;
    else
        return
            (*children)[address[current_address_position]]
                .get_target()
                .get_node_by_address(address, current_address_position+1);
}

void node::apply_simulation_result(const simulation_result& result){
    sum_of_scores += result;
    ++number_of_simulations;
}

void node::apply_simulation_result_for_address(const simulation_result& result, const node_address& address, uint current_address_position){
    if(current_address_position < address.size()){
        apply_simulation_result(result);
        (*children)[address[current_address_position]]
            .get_target()
            .apply_simulation_result_for_address(result, address, current_address_position+1);
    }
}

std::tuple<const reasoner::game_state&, bool> node::choose_state_for_simulation(node_address& current_address){
    if(number_of_attempts==0){
        ++number_of_attempts;
        return std::make_tuple(state, not is_terminal());
    }
    else{
        ++number_of_attempts;
        expand_children();
        if(children->empty())
            return std::make_pair(state, false);
        else{
            uint choice = children_with_highest_priority();
            (*children)[choice].create_target(*this);
            current_address.push_back(choice);
            return (*children)[choice].get_target().choose_state_for_simulation(current_address);
        }
    }
}

uint node::children_with_highest_priority(void)const{
    std::vector<priority> candidates;
    uint current_player = get_current_player();
    std::transform(children->begin(), children->end(), std::back_inserter(candidates),
        [this, current_player](const auto& el){return el.get_priority(number_of_simulations, current_player);});
    return std::distance(candidates.begin(), std::max_element(candidates.begin(), candidates.end()));
}

node node::create_node_after_move(const reasoner::move& m)const{
    reasoner::game_state result = state;
    result.apply_move(m);
    return node(cache, std::move(result), nodes_register);
}

const node& node::get_node_by_address(const node_address& address)const{
    return get_node_by_address(address, 0);
}

priority node::get_priority(uint parent_simulations, uint parent_player)const{
    if(number_of_simulations == 0)
        return {INF, number_of_attempts};
    else
        return {average_score(parent_player)/EXPECTED_MAX_SCORE+exploration_value(parent_simulations), number_of_attempts};
}

const reasoner::move& node::choose_best_move(void){
    assert(not is_terminal());
    expand_children();
    uint current_player = get_current_player();
    std::vector<double> children_to_choose;
    std::transform(children->begin(), children->end(), std::back_inserter(children_to_choose),
        [current_player](const auto& el){return el.average_score(current_player);});
    const auto chosen_child = std::max_element(children_to_choose.begin(), children_to_choose.end());
    const auto choice_number = std::distance(children_to_choose.begin(), chosen_child);
    return (*children)[choice_number].get_move();
}

std::optional<std::tuple<node_address, const reasoner::game_state&>> node::choose_state_for_simulation(void){
    for(uint i=0;i<MAX_TRIES_FOR_NON_TERMINAL_STATE;++i){
        node_address result;
        const auto& [state, suitable_for_simulation] = choose_state_for_simulation(result);
        if(suitable_for_simulation)
            return std::make_tuple(result, state);
        apply_simulation_result(simulation_result(state));
    }
    return std::nullopt;
}

void node::apply_simulation_result_for_address(const simulation_result& result, const node_address& address){
    apply_simulation_result_for_address(result, address, 0);
}

uint node::get_node_index_by_move(const reasoner::move& m){
    expand_children();
    const auto result = std::find_if(children->begin(), children->end(), [&m](const auto& el){return el.matches(m);});
    assert(result != children->end()); // told to move along nonexistant edge -- probably server bug
    return std::distance(children->begin(), result);
}

uint node::get_current_player(void)const{
    return state.get_current_player()-1;
}

bool node::is_terminal(void)const{
    return terminal;
}
