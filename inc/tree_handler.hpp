#ifndef TREE_HANDLER
#define TREE_HANDLER

#include"tree.hpp"
#include"types.hpp"

#include<string>

template<typename T>
class concurrent_queue;
class simulation_request;
class simulation_response;
class client_response;
class move_indication;

namespace reasoner{
    class game_state;
}

class tree_handler{
        tree t;
        uint own_player_index;
        std::vector<uint> moves_history = {};
        concurrent_queue<simulation_request>& requests_to_workers;
        concurrent_queue<client_response>& responses_to_server;
        void create_more_requests(void);
        bool address_still_usable(const simulation_response& response)const;
        node_address extract_usable_address(const simulation_response& response)const;
        uint game_turn(void)const;
    public:
        tree_handler(void)=delete;
        tree_handler(const tree_handler&)=delete;
        tree_handler(tree_handler&&)=default;
        tree_handler& operator=(const tree_handler&)=delete;
        tree_handler& operator=(tree_handler&&)=default;
        ~tree_handler(void)=default;
        tree_handler(const reasoner::game_state& initial_state,
                     const std::string& own_player_name,
                     concurrent_queue<simulation_request>& requests,
                     concurrent_queue<client_response>& responses_to_server);
        void handle_simulation_response(const simulation_response& response);
        void handle_move_request(void);
        void handle_move_indication(const reasoner::move& m);
        void handle_status_request(void);
};

#endif
