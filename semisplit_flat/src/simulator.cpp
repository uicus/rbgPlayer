#include"simulator.hpp"
#include"simulator_turn_handler.hpp"

simulation_result perform_simulation(reasoner::game_state& state,
                                     reasoner::resettable_bitarray_stack& cache,
                                     moves_container& legal_semimoves,
                                     std::mt19937& mt){
    while(handle_move(state, cache, legal_semimoves, mt));
    return simulation_result(state);
}

