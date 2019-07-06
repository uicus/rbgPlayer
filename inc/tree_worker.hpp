#ifndef TREE_WORKER
#define TREE_WORKER

#include<string>

template<typename T>
class concurrent_queue;
class simulation_request;
class tree_indication;
class client_response;

namespace reasoner{
    class game_state;
}

void run_tree_worker(const std::string& own_player_name,
                     concurrent_queue<simulation_request>& requests_to_workers,
                     concurrent_queue<client_response>& responses_to_server,
                     concurrent_queue<tree_indication>& tree_indications);

#endif
