#include"local_simulations_worker.hpp"
#include"reasoner.hpp"
#include"concurrent_queue.hpp"
#include"simulation_request.hpp"
#include"tree_indication.hpp"
#include"simulator.hpp"
#include"constants.hpp"
#include"moves_container.hpp"
#include<random>
#include<vector>

void run_local_simulations_worker(concurrent_queue<tree_indication>& responses_to_tree,
                                  concurrent_queue<simulation_request>& requests_from_tree,
                                  uint semimoves_length){
    reasoner::resettable_bitarray_stack cache;
    moves_container mc;
    std::random_device d;
    std::mt19937 mt(d());
    while(true){
        auto request = requests_from_tree.pop_front();
        auto result = perform_simulation(request.state, cache, mc, mt, semimoves_length);
        responses_to_tree.emplace_back_or_block_if_too_much(
            MAX_SIMULATION_RESPONSES,
            tree_indication{simulation_response{std::move(result), std::move(request.address), request.game_turn}});
    }
}
