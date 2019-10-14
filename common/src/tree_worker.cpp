#include"tree_worker.hpp"
#include"tree_handler.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"tree_indication.hpp"
#include"client_response.hpp"
#include"overloaded.hpp"

void run_tree_worker(const std::string& own_player_name,
                     concurrent_queue<simulation_request>& requests_to_workers,
                     concurrent_queue<client_response>& responses_to_server,
                     concurrent_queue<tree_indication>& tree_indications){
    reasoner::game_state initial_state;
    tree_handler th(initial_state, own_player_name, requests_to_workers, responses_to_server);
    while(true){
        const auto indication = tree_indications.pop_front();
        std::visit(overloaded{
            [&th](const simulation_response& response){th.handle_simulation_response(response);},
            [&th](const reasoner::move& m){th.handle_move_indication(m);},
            [&th](const move_request&){th.handle_move_request();},
            [&th](const status_request&){th.handle_status_request();},
            [](auto){assert(false);}
        }, indication.content);
    }
}
