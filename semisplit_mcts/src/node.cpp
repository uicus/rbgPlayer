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
    result.tstatus = tstatus;
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
        if(result.has_value())
            status = nondeadend;
        else{
            status = deadend;
            rating.apply_simulation_fail();
        }
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

std::vector<std::tuple<double,uint>> node::create_list_of_semichildren_for_move(state_tracker& tracker)const{
    std::vector<std::tuple<double, uint>> result;
    if(children.has_value())
        for(uint i=0;i<children->size();++i)
            if((*children)[i].has_target() and (*children)[i].get_target(tracker).status == nondeadend)
                result.emplace_back((*children)[i].average_score(tracker), i);
    return result;
}

void node::choose_best_move(reasoner::move& move_so_far, state_tracker& tracker){
    if(not tracker.get_state().is_nodal() or move_so_far.mr.empty()){
        auto choices = create_list_of_semichildren_for_move(tracker);
        if(choices.empty())
            tracker.complement_move(move_so_far);
        else{
            const auto chosen_child = std::get<1>(*std::max_element(choices.begin(), choices.end()));
            const auto& label = (*children)[chosen_child].get_label();
            tracker.go_along_semimove(label);
            move_so_far.mr.insert(move_so_far.mr.end(), label.get_actions().begin(), label.get_actions().end());
            (*children)[chosen_child].get_target(tracker).choose_best_move(move_so_far, tracker);
        }
    }
}

const reasoner::move node::choose_best_move(state_tracker& tracker){
     reasoner::move move_so_far = {};   
     choose_best_move(move_so_far, tracker);
     return move_so_far;
}

node_address node::choose_state_for_simulation(state_tracker& tracker){
    node_address result;
    choose_state_for_simulation(result, tracker);
    return result;
}

void node::apply_simulation_result_for_address(const simulation_result& result, const node_address& address, state_tracker& tracker){
    apply_simulation_result_for_address(result, address, 0, tracker);
}

std::tuple<uint, uint> node::get_child_match(const reasoner::move& m, uint current_move_position)const{
    for(uint i=0;i<children->size();++i){
        auto match_result = matches((*children)[i].get_label(), m, current_move_position);
        if(match_result.has_value())
            return {i, *match_result};
    }
    assert(false); // told to move along nonexistant edge -- probably server bug
}

void node::get_node_address_by_move(const reasoner::move& m, state_tracker& tracker, uint current_move_position, node_address& address_so_far){
    assert(current_move_position <= m.mr.size());
    if(m.mr.size() != current_move_position){
        if(not children)
            children = tracker.generate_children();
        auto [chosen_index, new_positon] = get_child_match(m, current_move_position);
        address_so_far.emplace_back(chosen_index);
        (*children)[chosen_index].create_target(tracker);
        tracker.go_along_semimove((*children)[chosen_index].get_label());
        (*children)[chosen_index]
            .get_target(tracker)
            .get_node_address_by_move(m, tracker, new_positon, address_so_far);
    }
}

node_address node::get_node_address_by_move(const reasoner::move& m, state_tracker& tracker){
    node_address address_so_far;
    get_node_address_by_move(m, tracker, 0, address_so_far);
    return address_so_far;
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
