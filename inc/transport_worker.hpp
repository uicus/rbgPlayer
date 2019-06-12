#ifndef TRANSPORT_WORKER
#define TRANSPORT_WORKER

template<typename T>
class concurrent_queue;
class client_response;
class tree_indication;

void run_transport_worker(// TODO: socket
                          concurrent_queue<tree_indication>& tree_indications,
                          concurrent_queue<client_response>& responses_from_tree);

#endif
