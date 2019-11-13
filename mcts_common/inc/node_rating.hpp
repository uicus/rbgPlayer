#ifndef NODE_RATING
#define NODE_RATING

#include"simulation_result.hpp"
#include"types.hpp"

class node_rating{
        uint number_of_simulations = 0;
        uint number_of_attempts = 0;
        simulation_result sum_of_scores = {};
        double exploration_value(uint parent_simulations)const;
    public:
        node_rating(void)=default;
        node_rating(const node_rating&)=default;
        node_rating(node_rating&&)=default;
        node_rating& operator=(const node_rating&)=default;
        node_rating& operator=(node_rating&&)=default;
        ~node_rating(void)=default;
        double average_score(uint player)const;
        priority get_priority(const node_rating& parent_rating, uint parent_player)const;
        bool ever_visited(void)const;
        void apply_simulation_result(const simulation_result& result);
        void apply_simulation_trial(void);
};

#endif
