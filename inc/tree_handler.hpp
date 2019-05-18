#ifndef TREE_HANDLER
#define TREE_HANDLER

#include"tree.hpp"
#include"types.hpp"

template<typename T>
class concurrent_queue;
class simulation_request;
class simulation_response;

namespace reasoner{
    class game_state;
}

class tree_handler{
        tree t;
        uint game_turn = 0;
        concurrent_queue<simulation_request>& requests;
        void create_more_requests(void);
    public:
        tree_handler(void)=delete;
        tree_handler(const tree_handler&)=delete;
        tree_handler(tree_handler&&)=default;
        tree_handler& operator=(const tree_handler&)=delete;
        tree_handler& operator=(tree_handler&&)=default;
        ~tree_handler(void)=default;
        tree_handler(const reasoner::game_state& initial_state, concurrent_queue<simulation_request>& requests);
        void handle_simulation_response(const simulation_response& response);
        // TODO: handle my move request
        // TODO: handle opponent move indication
};

#endif
