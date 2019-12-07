#include"state_tracker.hpp"
#include"node.hpp"
#include"edge.hpp"
#include"constants.hpp"
#include"moves_container.hpp"
#include"simulator_turn_handler.hpp"
#include<algorithm>
#include<cassert>

state_tracker::state_tracker(reasoner::resettable_bitarray_stack& cache,
                             std::vector<label_type>& children_label_container,
                             std::vector<node>& nodes_register,
                             std::mt19937& random_numbers_generator,
                             const reasoner::game_state& state)
  : cache(cache)
  , children_label_container(children_label_container)
  , nodes_register(nodes_register)
  , random_numbers_generator(random_numbers_generator)
  , state(state){}

void state_tracker::go_to_completion(void){
    while(state.get_current_player() == KEEPER)
        if(not state.apply_any_move(cache)){
            terminal = true;
            break;
        }
}

std::vector<edge> state_tracker::generate_children(void){
    if(terminal)
        return std::vector<edge>();
    else{
        std::uniform_real_distribution<double> random_distribution(-1.0, 1.0);
        std::vector<edge> children;
        state.get_all_semimoves(cache, children_label_container, SEMIMOVES_TREE_LENGTH);
        std::transform(children_label_container.begin(), children_label_container.end(), std::back_inserter(children),
            [this, &random_distribution](const auto& el){return edge(el, random_distribution(random_numbers_generator));});
        terminal = children.empty();
        return children;
    }
}

void state_tracker::go_along_semimove(const reasoner::semimove& m){
    terminal = false;
    semimove_reverts.emplace_back(state.apply_semimove_with_revert(m));
    go_to_completion();
    if(state.is_nodal())
        semimove_reverts.clear();
}

void state_tracker::revert_last_semimove(void){
    assert(not semimove_reverts.empty());
    state.revert(semimove_reverts.back());
    semimove_reverts.pop_back();
}

uint state_tracker::add_node_to_register(void){
    nodes_register.emplace_back();
    return nodes_register.size()-1;
}

const node& state_tracker::get_node(uint index)const{
    assert(index<nodes_register.size());
    return nodes_register[index];
}

node& state_tracker::get_node(uint index){
    assert(index<nodes_register.size());
    return nodes_register[index];
}

uint state_tracker::get_current_player(void)const{
    return state.get_current_player()-1;
}

const reasoner::game_state& state_tracker::get_state(void)const{
    return state;
}

bool state_tracker::has_any_legal_move(void){
    assert(state.is_nodal());
    moves_container legal_semimoves;
    return handle_move(state, cache, legal_semimoves, random_numbers_generator);
}

std::vector<reasoner::semimove> state_tracker::fill_semimoves_table(void){
    std::vector<reasoner::semimove> semimoves;
    state.get_all_semimoves(cache, semimoves, SEMIMOVES_TREE_LENGTH);
    return semimoves;
}

reasoner::revert_information state_tracker::apply_random_semimove_from_given(std::vector<reasoner::semimove>& semimoves,
                                                                             std::vector<reasoner::semimove>& chosen_semimoves){
    std::uniform_int_distribution<uint> distribution(0,semimoves.size()-1);
    uint chosen_semimove = distribution(random_numbers_generator);
    chosen_semimoves.emplace_back(semimoves[chosen_semimove]);
    reasoner::revert_information ri = state.apply_semimove_with_revert(semimoves[chosen_semimove]);
    semimoves[chosen_semimove] = semimoves.back();
    semimoves.pop_back();
    return ri;
}

bool state_tracker::apply_random_move_exhaustive(std::vector<reasoner::semimove>& chosen_semimoves){
    auto semimoves = fill_semimoves_table();
    while(not semimoves.empty()){
        auto ri = apply_random_semimove_from_given(semimoves, chosen_semimoves);
        if(state.is_nodal())
            return true;
        if(apply_random_move_exhaustive(chosen_semimoves))
            return true;
        state.revert(ri);
        chosen_semimoves.pop_back();
    }
    return false;
}

void state_tracker::complement_move(reasoner::move& move_so_far){
    std::vector<reasoner::semimove> chosen_semimoves;
    bool move_found = apply_random_move_exhaustive(chosen_semimoves);
    assert(move_found); // otherwise should not call this from this semistate
    for(const auto& el: chosen_semimoves)
        move_so_far.mr.insert(move_so_far.mr.end(), el.get_actions().begin(), el.get_actions().end());
}
