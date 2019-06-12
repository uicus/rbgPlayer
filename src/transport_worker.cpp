#include"transport_worker.hpp"
#include"concurrent_queue.hpp"
#include"client_response.hpp"
#include"tree_indication.hpp"
#include"constants.hpp"
#include<optional>
#include<chrono>
#include<thread>

namespace{
game_status_indication get_game_status(concurrent_queue<client_response>& responses_from_tree){
    auto response = responses_from_tree.pop_front();
    assert(std::holds_alternative<game_status_indication>(response.content));
    return std::get<game_status_indication>(response.content);
}

reasoner::move get_move_from_player(concurrent_queue<client_response>& responses_from_tree){
    auto response = responses_from_tree.pop_front();
    assert(std::holds_alternative<reasoner::move>(response.content));
    return std::get<reasoner::move>(response.content);
}

std::optional<reasoner::move> get_move(// TODO: socket
                                       game_status_indication status,
                                       concurrent_queue<tree_indication>& tree_indications,
                                       concurrent_queue<client_response>& responses_from_tree){
    switch(status){
        case own_turn:
            std::this_thread::sleep_for(std::chrono::milliseconds(MILISECONDS_PER_MOVE-BUFFER_TIME));
            tree_indications.emplace_back(tree_indication{move_request{}});
            return get_move_from_player(responses_from_tree);
        case opponent_turn:
            // TODO: parse incoming move from socket
        case end_game:
        default:
            return std::nullopt;
    }
}
}

void run_transport_worker(// TODO: socket
                          concurrent_queue<tree_indication>& tree_indications,
                          concurrent_queue<client_response>& responses_from_tree){
    game_status_indication current_status = own_turn; // TODO: retrieve correct value
    while(true){
        auto m = get_move(current_status,
                          tree_indications,
                          responses_from_tree);
        if(not m)
            return;
        tree_indications.emplace_back(tree_indication{std::move(*m)});
        current_status = get_game_status(responses_from_tree);
    }
}
