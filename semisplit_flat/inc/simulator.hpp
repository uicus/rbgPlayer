#ifndef SIMULATOR
#define SIMULATOR

#include<random>
#include<vector>

#include"moves_container.hpp"
#include"simulation_result.hpp"

namespace reasoner{
    class game_state;
    class move;
    class resettable_bitarray_stack;
}

simulation_result perform_simulation(reasoner::game_state& state,
                                     reasoner::resettable_bitarray_stack& cache,
                                     moves_container& legal_semimoves,
                                     std::mt19937& mt);

#endif
