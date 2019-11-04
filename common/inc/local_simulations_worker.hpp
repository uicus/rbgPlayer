#ifndef LOCAL_SIMULATIONS_WORKER
#define LOCAL_SIMULATIONS_WORKER

#include"types.hpp"

template<typename T>
class concurrent_queue;
class simulation_request;
class tree_indication;

void run_local_simulations_worker(concurrent_queue<tree_indication>& responses_to_tree,
                                  concurrent_queue<simulation_request>& requests_from_tree,
                                  uint semimoves_length);

#endif
