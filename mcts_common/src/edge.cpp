#include"edge.hpp"
#include"node.hpp"
#include"constants.hpp"
#include"state_tracker.hpp"

edge::edge(const reasoner::move& label)
  : label(label){}

edge edge::clone_edge(std::vector<node>& new_nodes_register, const state_tracker& tracker)const{
    edge result(label);
    if(target){
        auto&& cloned_target = tracker.get_node(*target).clone_node(new_nodes_register, tracker);
        new_nodes_register.emplace_back(std::move(cloned_target));
        result.target = new_nodes_register.size()-1;
    }
    return result;
}

void edge::create_target(state_tracker& tracker){
    if(not target)
        target = tracker.add_node_to_register();
}

const node& edge::get_target(const state_tracker& tracker)const{
    assert(target);
    return tracker.get_node(*target);
}

node& edge::get_target(state_tracker& tracker){
    assert(target);
    return tracker.get_node(*target);
}

priority edge::get_priority(uint parent_simulations, const state_tracker& tracker)const{
    if(not target)
        return INF;
    else
        return get_target(tracker).get_priority(parent_simulations, tracker.get_current_player());
}

bool edge::matches(const reasoner::move& m)const{
    return label == m;
}

double edge::average_score(const state_tracker& tracker)const{
    if(not target)
        return 0.0;
    else
        return get_target(tracker).average_score(tracker.get_current_player());
}

const reasoner::move& edge::get_label(void)const{
    return label;
}
