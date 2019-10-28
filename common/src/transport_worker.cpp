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

bool is_move_already_available(concurrent_queue<client_response>& responses_from_tree){
    return responses_from_tree.size() > 0;
}

void wait_for_move(uint milisecond_to_wait,
                   concurrent_queue<tree_indication>& tree_indications,
                   concurrent_queue<client_response>& responses_from_tree){
    uint time_left = milisecond_to_wait;
    std::chrono::steady_clock::time_point start_time(std::chrono::steady_clock::now());
    while(time_left > MILISECONDS_TIME_GRANULATION){
        std::this_thread::sleep_for(std::chrono::milliseconds(MILISECONDS_TIME_GRANULATION));
        if(is_move_already_available(responses_from_tree))
            return;
        std::chrono::steady_clock::time_point current_time(std::chrono::steady_clock::now());
        uint time_spent = std::chrono::duration_cast<std::chrono::milliseconds>(current_time-start_time).count();
        time_left = time_spent > milisecond_to_wait ? 0 : milisecond_to_wait-time_spent;
    }
    tree_indications.emplace_back(tree_indication{move_request{}});
}

std::chrono::steady_clock::time_point handle_turn(uint miliseconds_per_move,
                                                  remote_moves_receiver& rmr,
                                                  own_moves_sender& oms,
                                                  game_status_indication status,
                                                  concurrent_queue<tree_indication>& tree_indications,
                                                  concurrent_queue<client_response>& responses_from_tree){
    switch(status){
        case own_turn:
            wait_for_move(miliseconds_per_move-BUFFER_TIME, tree_indications, responses_from_tree);
            forward_move_from_player_to_server(oms, responses_from_tree);
            break;
        case opponent_turn:
            forward_move_from_server_to_player(rmr, tree_indications);
            break;
        case end_game:
            tree_indications.emplace_back(tree_indication{reset_tree()});
            break;
        default:
            assert(false);
    }
    return std::chrono::steady_clock::now();
}

uint count_miliseconds_left_for_current_turn(uint standard_wait_time,
                                             const std::chrono::steady_clock::time_point& current_turn_start_time)
{
    std::chrono::steady_clock::time_point current_time(std::chrono::steady_clock::now());
    uint time_wasted = std::chrono::duration_cast<std::chrono::milliseconds>(current_time-current_turn_start_time).count();
    return standard_wait_time-time_wasted;
}
}

void run_transport_worker(uint miliseconds_per_move,
                          remote_moves_receiver& rmr,
                          own_moves_sender& oms,
                          concurrent_queue<tree_indication>& tree_indications,
                          concurrent_queue<client_response>& responses_from_tree){
    game_status_indication current_status = get_game_status(responses_from_tree);
    std::chrono::steady_clock::time_point current_turn_start_time(std::chrono::steady_clock::now());
    while(true){
        current_turn_start_time = handle_turn(count_miliseconds_left_for_current_turn(miliseconds_per_move, current_turn_start_time),
                                              rmr,
                                              oms,
                                              current_status,
                                              tree_indications,
                                              responses_from_tree);
        current_status = get_game_status(responses_from_tree);
    }
}
