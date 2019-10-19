#ifndef TREE_HISTORY
#define TREE_HISTORY

#include"node_address.hpp"
#include"types.hpp"

class simulation_response;

class tree_history{
        uint current_turn = 0;
    public:
        tree_history(void)=default;
        tree_history(const tree_history&)=default;
        tree_history(tree_history&&)=default;
        tree_history& operator=(const tree_history&)=default;
        tree_history& operator=(tree_history&&)=default;
        ~tree_history(void)=default;
        uint game_turn(void)const;
        bool address_fully_usable(const simulation_response& response)const;
        bool address_partially_usable(const simulation_response& response)const;
        node_address extract_usable_address(const simulation_response& response)const;
        void notify_about_move(uint);
};

#endif
