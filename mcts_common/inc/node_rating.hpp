#ifndef NODE_RATING
#define NODE_RATING

#include<random>

#include"result_container.hpp"
#include"types.hpp"

class node_rating{
        uint number_of_simulations = 0;
        uint number_of_attempts = 0;
        result_container sum_of_scores = {};
        double random_score_offset = 0.0;
        double exploration_value(uint parent_simulations)const;
    public:
        node_rating(void)=delete;
        node_rating(const node_rating&)=default;
        node_rating(node_rating&&)=default;
        node_rating& operator=(const node_rating&)=default;
        node_rating& operator=(node_rating&&)=default;
        node_rating(std::mt19937& random_numbers_generator);
        ~node_rating(void)=default;
        double average_score(uint player)const;
        priority get_priority(const node_rating& parent_rating, uint parent_player)const;
        bool ever_visited(void)const;
        void apply_simulation_result(const result_container& result);
        void apply_simulation_trial(void);
        void apply_simulation_fail(void);
        void apply_simulation_trial_revert(void);
};

#endif
