#include"transport_worker.hpp"
#include"concurrent_queue.hpp"
#include"client_response.hpp"
#include"tree_indication.hpp"
#include"constants.hpp"
#include"remote_moves_receiver.hpp"
#include"own_moves_sender.hpp"
#include<optional>
#include<chrono>
#include<thread>

namespace{
game_status_indication get_game_status(concurrent_queue<client_response>& responses_from_tree){
    auto response = responses_from_tree.pop_front();
    assert(std::holds_alternative<game_status_indication>(response.content));
    return std::get<game_status_indication>(response.content);
}

game_status_indication get_initial_game_status(concurrent_queue<tree_indication>& tree_indications,
                                               concurrent_queue<client_response>& responses_from_tree){
    tree_indications.emplace_back(tree_indication{status_request()});
    return get_game_status(responses_from_tree);
}

reasoner::move get_move_from_player(concurrent_queue<client_response>& responses_from_tree){
    auto response = responses_from_tree.pop_front();
    assert(std::holds_alternative<reasoner::move>(response.content));
    return std::get<reasoner::move>(response.content);
}

void forward_move_from_player_to_server(own_moves_sender& oms,
                                        concurrent_queue<client_response>& responses_from_tree){
    auto m = get_move_from_player(responses_from_tree);
    oms.send_move(m);
}

void forward_move_from_server_to_player(remote_moves_receiver& rmr,
                                        concurrent_queue<tree_indication>& tree_indications){
    auto m = rmr.receive_move();
    tree_indications.emplace_back(tree_indication{m});
}

void handle_turn(remote_moves_receiver& rmr,
                 own_moves_sender& oms,
                 game_status_indication status,
                 concurrent_queue<tree_indication>& tree_indications,
                 concurrent_queue<client_response>& responses_from_tree){
    switch(status){
        case own_turn:
            std::this_thread::sleep_for(std::chrono::milliseconds(MILISECONDS_PER_MOVE-BUFFER_TIME));
            tree_indications.emplace_back(tree_indication{move_request{}});
            forward_move_from_player_to_server(oms, responses_from_tree);
            break;
        case opponent_turn:
            forward_move_from_server_to_player(rmr, tree_indications);
            break;
        case end_game:
        default:
            assert(false);
    }
}
}

void run_transport_worker(remote_moves_receiver& rmr,
                          own_moves_sender& oms,
                          concurrent_queue<tree_indication>& tree_indications,
                          concurrent_queue<client_response>& responses_from_tree){
    game_status_indication current_status = get_initial_game_status(tree_indications, responses_from_tree);
    while(current_status != end_game){
        handle_turn(rmr,
                    oms,
                    current_status,
                    tree_indications,
                    responses_from_tree);
        current_status = get_game_status(responses_from_tree);
    }
}
