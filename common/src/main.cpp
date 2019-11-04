#include"concurrent_queue.hpp"

#include"client_response.hpp"
#include"tree_indication.hpp"
#include"simulation_request.hpp"

#include"transport_worker.hpp"
#include"tree_worker.hpp"
#include"local_simulations_worker.hpp"

#include"own_moves_sender.hpp"
#include"remote_moves_receiver.hpp"

#include<cassert>
#include<thread>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"types.hpp"

int connect_to_play(const std::string& address, uint port){
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor < 0)
        return 0;
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, address.data(), &server_address.sin_addr);
    auto connect_result = connect(socket_descriptor, (sockaddr*)&server_address, sizeof(server_address));
    if(connect_result < 0)
        return 0;
    return socket_descriptor;
}

int main(int argc, char** argv){
    assert(argc == 8); // otherwise bug in play.py
    std::string play_address = argv[1];
    uint play_port = std::stoi(argv[2]);
    std::string player_name = argv[3];
    uint number_of_simulation_threads = std::stoi(argv[4]);
    uint miliseconds_per_move = std::stoi(argv[5]);
    uint simulations_limit = std::stoi(argv[6]);
    uint semimoves_length = std::stoi(argv[7]);
    concurrent_queue<client_response> client_responses;
    concurrent_queue<tree_indication> tree_indications;
    concurrent_queue<simulation_request> simulation_requests;
    auto socket_descriptor = connect_to_play(play_address, play_port);
    if(not socket_descriptor)
        return -1;
    remote_moves_receiver rmr(socket_descriptor);
    own_moves_sender oms(socket_descriptor);
    std::thread transportw(run_transport_worker, miliseconds_per_move, std::ref(rmr), std::ref(oms), std::ref(tree_indications), std::ref(client_responses));
    std::thread treew(run_tree_worker, player_name, simulations_limit, std::ref(simulation_requests), std::ref(client_responses), std::ref(tree_indications));
    std::vector<std::thread> simulationw;
    for(uint i=0;i<number_of_simulation_threads;++i)
        simulationw.emplace_back(run_local_simulations_worker, std::ref(tree_indications), std::ref(simulation_requests), semimoves_length);
    transportw.join();
    close(socket_descriptor);
    treew.join(); // in fact just waiting to be terminated
    for(auto& el: simulationw)
        el.join();
    return 0;
}
