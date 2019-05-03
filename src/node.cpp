#include"node.hpp"
#include"constants.hpp"

node::node(reasoner::resettable_bitarray_stack& cache, const reasoner::game_state& state)
  : cache(cache)
  , state(state)
  , children()
  , terminal(false){
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

void node::expand_children(void){
    children.clear();
    auto all_moves = state.get_all_moves(cache);
    for(const auto& m: all_moves){
        reasoner::game_state next_state = state;
        next_state.apply_move(m);
        children.emplace_back(m, std::unique_ptr<node>(new node(cache, next_state)));
    }
}
