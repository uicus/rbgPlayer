#include"local_worker.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"simulation_response.hpp"
#include"simulator.hpp"
#include<random>

void run_local_worker(concurrent_queue<simulation_request>& requests,
                      concurrent_queue<simulation_response>& responses){
    reasoner::resettable_bitarray_stack cache;
    std::random_device d;
    std::mt19937 mt(d());
    while(true){
        auto request = requests.pop_front();
        auto result = perform_simulation(request.state, cache, mt);
        responses.emplace_back(simulation_response{std::move(result), std::move(request.address), request.game_turn});
    }
}
