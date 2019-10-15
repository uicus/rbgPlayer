#include"state_tracker.hpp"
#include"node.hpp"
#include"edge.hpp"
#include"constants.hpp"
#include<algorithm>
#include<cassert>

state_tracker::state_tracker(reasoner::resettable_bitarray_stack& cache,
                             std::vector<node>& nodes_register,
                             const reasoner::game_state& state)
  : cache(cache)
  , nodes_register(nodes_register)
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
        std::vector<edge> children;
        auto all_moves = state.get_all_moves(cache);
        std::transform(all_moves.begin(), all_moves.end(), std::back_inserter(children),
            [this](const auto& el){return edge(el);});
        terminal = children.empty();
        return children;
    }
}

void state_tracker::go_along_move(const reasoner::move& m){
    terminal = false;
    state.apply_move(m);
    go_to_completion();
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
