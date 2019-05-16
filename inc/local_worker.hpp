#ifndef LOCAL_WORKER
#define LOCAL_WORKER

#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"

void run_local_worker(concurrent_queue<simulation_request>& requests,
                      concurrent_queue<simulation_response>& responses);

#endif
