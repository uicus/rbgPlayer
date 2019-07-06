#include"node.hpp"
#include"constants.hpp"
#include"state_tracker.hpp"
#include"edge.hpp"
#include<cmath>
#include<algorithm>

node node::clone_node(std::vector<node>& new_nodes_register, const state_tracker& tracker)const{
    node result;
    result.number_of_simulations = number_of_simulations;
    result.number_of_attempts = number_of_attempts;
    result.sum_of_scores = sum_of_scores;
    if(children){
        result.children = std::vector<edge>();
        std::transform(children->begin(), children->end(), std::back_inserter(*result.children),
            [&new_nodes_register, &tracker](const auto& el){return el.clone_edge(new_nodes_register, tracker);});
    }
    return result;
}

double node::average_score(uint player)const{
    if(number_of_simulations == 0)
        return EXPECTED_MAX_SCORE;
    else
        return sum_of_scores.get_player_score_divided_by(player, number_of_simulations);
}

double node::exploration_value(uint parent_simulations)const{
    return EXPLORATION_CONSTANT*sqrt(log(double(parent_simulations))/double(number_of_attempts));
}

const node& node::get_node_by_address(const node_address& address, uint current_address_position, state_tracker& tracker)const{
    if(current_address_position >= address.size())
        return *this;
    else{
        tracker.go_along_move((*children)[address[current_address_position]].get_move());
        return
            (*children)[address[current_address_position]]
                .get_target(tracker)
                .get_node_by_address(address, current_address_position+1, tracker);
    }
}

void node::apply_simulation_result(const simulation_result& result){
    sum_of_scores += result;
    ++number_of_simulations;
}

void node::apply_simulation_result_for_address(const simulation_result& result,
                                               const node_address& address,
                                               uint current_address_position,
                                               state_tracker& tracker){
    apply_simulation_result(result);
    if(current_address_position < address.size()){
        auto& n = 
        (*children)[address[current_address_position]]
            .get_target(tracker);
        n.apply_simulation_result_for_address(result, address, current_address_position+1, tracker);
    }
}

void node::choose_state_for_simulation(node_address& current_address, state_tracker& tracker){
    if(number_of_attempts==0)
        ++number_of_attempts;
    else{
        if(not children)
            children = tracker.generate_children();
        if(children->empty())
            ++number_of_attempts;
        else{
            uint choice = children_with_highest_priority(tracker);
            ++number_of_attempts;
            (*children)[choice].create_target(tracker);
            // std::cout<<"SZUKANIE:"<<std::endl;
            // std::cout<<"Pozycja: "<<current_address.size()<<std::endl;
            // std::cout<<"Pole: "<<choice<<" Rozmiar: "<<children->size()<<std::endl;
            current_address.push_back(choice);
            tracker.go_along_move((*children)[choice].get_move());
            (*children)[choice]
                .get_target(tracker)
                .choose_state_for_simulation(current_address, tracker);
        }
    }
}

uint node::children_with_highest_priority(const state_tracker& tracker)const{
    std::vector<priority> candidates;
    std::transform(children->begin(), children->end(), std::back_inserter(candidates),
        [this, &tracker](const auto& el){return el.get_priority(number_of_attempts, tracker);});
    return std::distance(candidates.begin(), std::max_element(candidates.begin(), candidates.end()));
}

const node& node::get_node_by_address(const node_address& address, state_tracker& tracker)const{
    return get_node_by_address(address, 0, tracker);
}

priority node::get_priority(uint parent_simulations, uint parent_player)const{
    if(number_of_attempts == 0)
        return INF;
    else
        return average_score(parent_player)/EXPECTED_MAX_SCORE+exploration_value(parent_simulations);
}

const reasoner::move& node::choose_best_move(const state_tracker& tracker){
    assert(children and not children->empty());
    std::vector<double> children_to_choose;
    std::transform(children->begin(), children->end(), std::back_inserter(children_to_choose),
        [&tracker](const auto& el){return el.average_score(tracker);});
    const auto chosen_child = std::max_element(children_to_choose.begin(), children_to_choose.end());
    const auto choice_number = std::distance(children_to_choose.begin(), chosen_child);
    return (*children)[choice_number].get_move();
}

node_address node::choose_state_for_simulation(state_tracker& tracker){
    node_address result;
    choose_state_for_simulation(result, tracker);
    return result;
}

void node::apply_simulation_result_for_address(const simulation_result& result, const node_address& address, state_tracker& tracker){
    apply_simulation_result_for_address(result, address, 0, tracker);
}

uint node::get_node_index_by_move(const reasoner::move& m){
    assert(children);
    const auto result = std::find_if(children->begin(), children->end(), [&m](const auto& el){return el.matches(m);});
    assert(result != children->end()); // told to move along nonexistant edge -- probably server bug
    return std::distance(children->begin(), result);
}

bool node::is_terminal(void)const{
    return children and children->empty();
}
