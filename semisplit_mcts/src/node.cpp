#include"node.hpp"
#include"constants.hpp"
#include"state_tracker.hpp"
#include"edge.hpp"
#include<algorithm>

node node::clone_node(std::vector<node>& new_nodes_register, const state_tracker& tracker)const{
    node result;
    result.rating = rating;
    result.status = status;
    if(children){
        result.children = std::vector<edge>();
        std::transform(children->begin(), children->end(), std::back_inserter(*result.children),
            [&new_nodes_register, &tracker](const auto& el){return el.clone_edge(new_nodes_register, tracker);});
    }
    return result;
}

const node& node::get_node_by_address(const node_address& address, uint current_address_position, state_tracker& tracker){
    if(current_address_position >= address.size())
        return *this;
    else{
        tracker.go_along_semimove((*children)[address[current_address_position]].get_label());
        (*children)[address[current_address_position]].create_target(tracker);
        return
            (*children)[address[current_address_position]]
                .get_target(tracker)
                .get_node_by_address(address, current_address_position+1, tracker);
    }
}

void node::apply_simulation_result_for_address(const simulation_result& result,
                                               const node_address& address,
                                               uint current_address_position,
                                               state_tracker& tracker){
    if(result.has_value())
        rating.apply_simulation_result(*result);
    if(current_address_position < address.size())
        (*children)[address[current_address_position]]
            .get_target(tracker)
            .apply_simulation_result_for_address(result, address, current_address_position+1, tracker);
    if(status == simulation_ongoing and current_address_position == address.size())
        status = result.has_value() ? nonterminal : terminal;
}

void node::choose_state_for_simulation(node_address& current_address, state_tracker& tracker){
    assert(false); // TODO
}

uint node::children_with_highest_priority(const state_tracker& tracker)const{
    std::vector<priority> candidates;
    std::transform(children->begin(), children->end(), std::back_inserter(candidates),
        [this, &tracker](const auto& el){return el.get_priority(rating, tracker);});
    return std::distance(candidates.begin(), std::max_element(candidates.begin(), candidates.end()));
}

const node& node::get_node_by_address(const node_address& address, state_tracker& tracker){
    return get_node_by_address(address, 0, tracker);
}

const reasoner::move& node::choose_best_move(const state_tracker& tracker){
    assert(false); // TODO
}

node_address node::choose_state_for_simulation(state_tracker& tracker){
    node_address result;
    choose_state_for_simulation(result, tracker);
    return result;
}

void node::apply_simulation_result_for_address(const simulation_result& result, const node_address& address, state_tracker& tracker){
    apply_simulation_result_for_address(result, address, 0, tracker);
}

node_address node::get_node_address_by_move(const reasoner::move& m, state_tracker& tracker){
    assert(false); // TODO
}

bool node::is_terminal(state_tracker& tracker){
    if(status == unknown or status == simulation_ongoing){
        if(tracker.has_any_legal_move())
            status = nonterminal;
        else
            status = terminal;
    }
    return status == terminal;
}

const node_rating& node::get_rating(void)const{
    return rating;
}
