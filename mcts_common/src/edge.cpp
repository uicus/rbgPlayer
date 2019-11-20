#include"edge.hpp"
#include"node.hpp"
#include"node_rating.hpp"
#include"constants.hpp"
#include"state_tracker.hpp"

edge::edge(const label_type& label)
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

bool edge::has_target(void)const{
    return target.has_value();
}

priority edge::get_priority(const node_rating& parent_rating, const state_tracker& tracker)const{
    if(not target)
        return INF;
    else
        return get_target(tracker).get_rating().get_priority(parent_rating, tracker.get_current_player());
}

double edge::average_score(const state_tracker& tracker)const{
    if(not target)
        return 0.0;
    else
        return get_target(tracker).get_rating().average_score(tracker.get_current_player());
}

const label_type& edge::get_label(void)const{
    return label;
}
