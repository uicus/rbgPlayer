#include"tree_worker.hpp"
#include"tree_handler.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"tree_indication.hpp"
#include"client_response.hpp"
#include"overloaded.hpp"

void run_tree_worker(concurrent_queue<simulation_request>& requests_to_workers,
                     concurrent_queue<client_response>& responses_to_server,
                     concurrent_queue<tree_indication>& tree_indications){
    reasoner::game_state initial_state;
    tree_handler th(initial_state, requests_to_workers, responses_to_server);
    while(true){
        const auto indication = tree_indications.pop_front();
        std::visit(overloaded{
            [&th](const simulation_response& response){th.handle_simulation_response(response);},
            [&th](const reasoner::move& m){th.handle_move_indication(m);},
            [&th](const move_request&){th.handle_move_request();},
            [&th,&initial_state](const reset_tree&){th.handle_reset_request(initial_state);},
            [](auto){assert(false);}
        }, indication.content);
    }
}
