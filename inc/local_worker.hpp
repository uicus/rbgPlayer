#ifndef LOCAL_WORKER
#define LOCAL_WORKER

template<typename T>
class concurrent_queue;
class simulation_request;
class simulation_response;

void run_local_worker(concurrent_queue<simulation_request>& requests_from_tree,
                      concurrent_queue<simulation_response>& responses_to_tree);

#endif
