#include"local_worker.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"tree_indication.hpp"
#include"simulator.hpp"
#include<random>

void run_local_worker(concurrent_queue<tree_indication>& responses_to_tree,
                      concurrent_queue<simulation_request>& requests_from_tree){
    reasoner::resettable_bitarray_stack cache;
    std::random_device d;
    std::mt19937 mt(d());
    while(true){
        auto request = requests_from_tree.pop_front();
        auto result = perform_simulation(request.state, cache, mt);
        responses_to_tree.emplace_back(tree_indication{simulation_response{std::move(result), std::move(request.address), request.game_turn}});
    }
}
