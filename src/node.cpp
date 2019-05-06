#include"node.hpp"
#include"constants.hpp"

node::node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state)
  : cache(cache)
  , state(state){
    go_to_completion();
}

void node::go_to_completion(void){
    while(state.get_current_player() == KEEPER and not terminal){
        auto any_move = state.get_any_move(cache);
        if(any_move)
            state.apply_move(*any_move);
        else
            terminal = true;
    }
}

double node::average_score(uint player)const{
    return sum_of_scores.get_player_score_divided_by(player, number_of_simulations);
}

double node::average_current_player_score(void)const{
    return average_score(state.get_current_player()-1);
}

void node::expand_children(void){
    children.clear();
    auto all_moves = state.get_all_moves(cache);
    for(const auto& m: all_moves){
        reasoner::game_state next_state = state;
        next_state.apply_move(m);
        children.emplace_back(m, std::unique_ptr<node>(new node(cache, next_state)));
    }
}

namespace{
bool handle_keeper_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache){
    auto any_move = state.get_any_move(cache);
    if(any_move){
        state.apply_move(*any_move);
        return true;
    }
    else
        return false;
}

bool handle_player_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache, std::mt19937& mt){
    auto legal_moves = state.get_all_moves(cache);
    if(not legal_moves.empty()){
        std::uniform_int_distribution<> random_distribution(0, legal_moves.size()-1);
        state.apply_move(legal_moves[random_distribution(mt)]);
        return true;
    }
    else
        return false;
}

bool handle_move(reasoner::game_state& state, reasoner::resettable_bitarray_stack& cache, std::mt19937& mt){
    if(state.get_current_player() == KEEPER)
        return handle_keeper_move(state, cache);
    else
        return handle_player_move(state, cache, mt);
}
}

simulation_result node::perform_simulation(std::mt19937& mt)const{
    if(terminal)
        return simulation_result(state);
    else{
        reasoner::game_state working_copy_state = state;
        while(handle_move(working_copy_state, cache, mt));
        return simulation_result(working_copy_state);
    }
}

void node::apply_simulation_result(const simulation_result& result){
    sum_of_scores += result;
    ++number_of_simulations;
}
