#include"node.hpp"
#include"constants.hpp"
#include"state_tracker.hpp"
#include"edge.hpp"
#include<algorithm>

node::node(void)
  : status(unexplored)
  , tstatus(unknown){
}

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
    else
        rating.apply_simulation_trial_revert();
    if(current_address_position < address.size())
        (*children)[address[current_address_position]]
            .get_target(tracker)
            .apply_simulation_result_for_address(result, address, current_address_position+1, tracker);
    if(status == simulation_ongoing and current_address_position == address.size()){
        status = result.has_value() ? nondeadend : deadend;
        rating.apply_simulation_fail();
    }
}

void node::choose_state_for_simulation(node_address& current_address, state_tracker& tracker){
    if(rating.ever_visited()){
        if(not children)
            children = tracker.generate_children();
        if(not children->empty()){
            uint choice = children_with_highest_priority(tracker);
            (*children)[choice].create_target(tracker);
            current_address.push_back(choice);
            tracker.go_along_semimove((*children)[choice].get_label());
            (*children)[choice]
                .get_target(tracker)
                .choose_state_for_simulation(current_address, tracker);
        }
    }
    else if(status == unexplored)
        status = simulation_ongoing;
    rating.apply_simulation_trial();
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
    if(tstatus == unknown){
        if(tracker.has_any_legal_move())
            tstatus = nonterminal;
        else
            tstatus = terminal;
    }
    return tstatus == terminal;
}

const node_rating& node::get_rating(void)const{
    return rating;
}
