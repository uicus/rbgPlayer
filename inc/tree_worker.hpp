#ifndef TREE_WORKER
#define TREE_WORKER

template<typename T>
class concurrent_queue;
class simulation_request;
class simulation_response;

namespace reasoner{
    class game_state;
}

void run_tree_worker(const reasoner::game_state& initial_state,
                     concurrent_queue<simulation_request>& requests,
                     concurrent_queue<simulation_response>& responses);

#endif
